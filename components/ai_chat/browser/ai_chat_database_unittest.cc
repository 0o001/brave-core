/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/ai_chat/browser/ai_chat_database.h"

#include <stdint.h>
#include <utility>
#include <vector>

#include "base/files/file_util.h"
#include "base/files/scoped_temp_dir.h"
#include "base/path_service.h"
#include "base/task/thread_pool.h"
#include "base/test/bind.h"
#include "base/test/task_environment.h"
#include "base/threading/sequence_bound.h"
#include "sql/test/test_helpers.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace {
int64_t GetInternalValue(const base::Time& time) {
  return time.ToDeltaSinceWindowsEpoch().InMicroseconds();
}
}  // namespace

namespace ai_chat {

class AIChatDatabaseTest : public testing::Test {
 public:
  AIChatDatabaseTest()
      : task_environment_(base::test::TaskEnvironment(
            base::test::TaskEnvironment::ThreadPoolExecutionMode::QUEUED)),
        db_(base::SequenceBound<AIChatDatabase>(CreateTaskRunner())) {}

  void SetUp() override { ASSERT_TRUE(temp_directory_.CreateUniqueTempDir()); }
  void TearDown() override { ASSERT_TRUE(temp_directory_.Delete()); }

  base::FilePath db_dir() { return temp_directory_.GetPath(); }

  base::FilePath db_file_path() {
    return temp_directory_.GetPath().Append("ai_chat");
  }

 protected:
  scoped_refptr<base::SequencedTaskRunner> CreateTaskRunner() {
    return base::ThreadPool::CreateSequencedTaskRunner(
        {base::MayBlock(), base::TaskPriority::BEST_EFFORT,
         base::ThreadPolicy::PREFER_BACKGROUND});
  }

  template <typename Func>
  void InitDBAndDoWork(Func operation) {
    db_.AsyncCall(&AIChatDatabase::Init)
        .WithArgs(db_file_path())
        .Then(base::BindLambdaForTesting([&](bool success) {
          ASSERT_TRUE(success);  // 0 is INIT_OK
          ASSERT_TRUE(base::PathExists(db_file_path()));
          operation();
        }));

    task_environment_.RunUntilIdle();
  }

  base::ScopedTempDir temp_directory_;
  base::test::TaskEnvironment task_environment_;
  base::SequenceBound<AIChatDatabase> db_;
};

TEST_F(AIChatDatabaseTest, AddConversations) {
  InitDBAndDoWork([&]() {
    auto on_get_all_conversations =
        [&](std::vector<mojom::ConversationPtr> conversations) {
          EXPECT_EQ(conversations.size(), UINT64_C(2));
        };

    db_.AsyncCall(base::IgnoreResult(&AIChatDatabase::AddConversation))
        .WithArgs(mojom::Conversation::New(INT64_C(1), base::Time::Now(),
                                           "Initial conversation", GURL()))
        .Then(base::BindLambdaForTesting([&]() {
          db_.AsyncCall(base::IgnoreResult(&AIChatDatabase::AddConversation))
              .WithArgs(mojom::Conversation::New(INT64_C(2), base::Time::Now(),
                                                 "Another conversation",
                                                 GURL()))
              .Then(base::BindLambdaForTesting([&]() {
                db_.AsyncCall(&AIChatDatabase::GetAllConversations)
                    .Then(base::BindLambdaForTesting(on_get_all_conversations));
              }));
        }));
  });
}

TEST_F(AIChatDatabaseTest, AddConversationEntry) {
  // We need static storage variables throughout the depths of lambdas
  static const int64_t kConversationId = INT64_C(1);
  static const int64_t kConversationEntryId = INT64_C(1);

  static const GURL kURL = GURL("https://brave.com/");
  static const char kConversationTitle[] = "Summarizing Brave";

  static const char kFirstResponse[] = "This is a generated response";
  static const char kSecondResponse[] = "This is a re-generated response";

  static const base::Time kFirstTextCreatedAt(base::Time::Now());
  static const base::Time kSecondTextCreatedAt(base::Time::Now() +
                                               base::Minutes(5));

  static std::vector<mojom::ConversationEntryTextPtr> kTexts;
  kTexts.emplace_back(mojom::ConversationEntryText::New(1, kFirstTextCreatedAt,
                                                        kFirstResponse));
  kTexts.emplace_back(mojom::ConversationEntryText::New(2, kSecondTextCreatedAt,
                                                        kSecondResponse));

  InitDBAndDoWork([&]() {
    db_.AsyncCall(&AIChatDatabase::AddConversation)
        .WithArgs(mojom::Conversation::New(kConversationId, kFirstTextCreatedAt,
                                           kConversationTitle, kURL))
        .Then(base::BindLambdaForTesting([&](int64_t conversation_id) {
          EXPECT_EQ(conversation_id, kConversationId);

          db_.AsyncCall(&AIChatDatabase::AddConversationEntry)
              .WithArgs(conversation_id,
                        mojom::ConversationEntry::New(
                            INT64_C(-1), kFirstTextCreatedAt,
                            mojom::CharacterType::ASSISTANT, std::move(kTexts)))
              .Then(base::BindLambdaForTesting([&](int64_t entry_id) {
                EXPECT_EQ(entry_id, kConversationEntryId);

                db_.AsyncCall(&AIChatDatabase::GetAllConversations)
                    .Then(base::BindLambdaForTesting(
                        [&](std::vector<mojom::ConversationPtr> conversations) {
                          EXPECT_FALSE(conversations.empty());
                          EXPECT_EQ(conversations[0]->id, INT64_C(1));
                          EXPECT_EQ(conversations[0]->title,
                                    kConversationTitle);
                          EXPECT_EQ(conversations[0]->page_url->spec(),
                                    kURL.spec());
                          EXPECT_EQ(GetInternalValue(conversations[0]->date),
                                    GetInternalValue(kFirstTextCreatedAt));

                          db_.AsyncCall(&AIChatDatabase::GetConversationEntries)
                              .WithArgs(kConversationId)
                              .Then(base::BindLambdaForTesting(
                                  [&](std::vector<mojom::ConversationEntryPtr>
                                          entries) {
                                    EXPECT_EQ(UINT64_C(1), entries.size());
                                    EXPECT_EQ(UINT64_C(2),
                                              entries[0]->texts.size());

                                    EXPECT_EQ(entries[0]->character_type,
                                              mojom::CharacterType::ASSISTANT);

                                    // ConversationEntry's date should match
                                    // first generated text's date
                                    EXPECT_EQ(
                                        GetInternalValue(entries[0]->date),
                                        GetInternalValue(kFirstTextCreatedAt));

                                    EXPECT_EQ(entries[0]->texts[0]->text,
                                              kFirstResponse);
                                    EXPECT_EQ(entries[0]->texts[1]->text,
                                              kSecondResponse);
                                  }));
                        }));
              }));
        }));
  });
}

}  // namespace ai_chat
