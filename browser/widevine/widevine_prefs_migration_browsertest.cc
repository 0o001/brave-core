/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/widevine/widevine_utils.h"
#include "brave/components/constants/pref_names.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/profile.h"
#include "components/prefs/pref_service.h"
#include "content/public/test/browser_test.h"

#if BUILDFLAG(IS_ANDROID)
#include "chrome/test/base/android/android_browser_test.h"
#include "chrome/test/base/chrome_test_utils.h"
#else
#include "chrome/browser/ui/browser.h"
#include "chrome/test/base/in_process_browser_test.h"
#endif

namespace {
constexpr bool kWidevineOptedInTestValue = true;
}  // namespace

class WidevinePrefsMigrationTest : public PlatformBrowserTest {
 public:
  WidevinePrefsMigrationTest() {}

  Profile* GetProfile() {
#if BUILDFLAG(IS_ANDROID)
    return chrome_test_utils::GetProfile(this);
#else
    return browser()->profile();
#endif
  }
};

IN_PROC_BROWSER_TEST_F(WidevinePrefsMigrationTest, PrefMigrationTest) {
  g_browser_process->local_state()->ClearPref(kWidevineOptedIn);
  EXPECT_TRUE(g_browser_process->local_state()->
      FindPreference(kWidevineOptedIn)->IsDefaultValue());

  // Set profile prefs explicitly for migration test.
  GetProfile()->GetPrefs()->SetBoolean(kWidevineOptedIn,
                                       kWidevineOptedInTestValue);

  // Migrate and check it's done properly with previous profile prefs value.
  MigrateWidevinePrefs(GetProfile());
  EXPECT_FALSE(g_browser_process->local_state()->
      FindPreference(kWidevineOptedIn)->IsDefaultValue());
  EXPECT_EQ(kWidevineOptedInTestValue,
            g_browser_process->local_state()->GetBoolean(kWidevineOptedIn));
}
