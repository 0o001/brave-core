/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_ads/core/internal/history/category_content_util.h"

#include "brave/components/brave_ads/core/common/interfaces/brave_ads.mojom-shared.h"
#include "brave/components/brave_ads/core/internal/ads/ad_unittest_constants.h"
#include "brave/components/brave_ads/core/internal/common/unittest/unittest_base.h"

// npm run test -- brave_unit_tests --filter=BraveAds*

namespace brave_ads {

class BraveAdsCategoryContentUtilTest : public UnitTestBase {};

TEST_F(BraveAdsCategoryContentUtilTest, Build) {
  // Arrange

  // Act
  const CategoryContentInfo category_content = BuildCategoryContent(kSegment);

  // Assert
  CategoryContentInfo expected_category_content;
  expected_category_content.category = kSegment;
  expected_category_content.user_reaction_type =
      mojom::UserReactionType::kNeutral;

  EXPECT_EQ(expected_category_content, category_content);
}

}  // namespace brave_ads
