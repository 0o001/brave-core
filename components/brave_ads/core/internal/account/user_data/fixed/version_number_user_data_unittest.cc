/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_ads/core/internal/account/user_data/fixed/version_number_user_data.h"

#include <string>

#include "base/strings/string_util.h"
#include "base/test/values_test_util.h"
#include "brave/components/brave_ads/core/internal/ads/ad_unittest_util.h"
#include "brave/components/brave_ads/core/internal/browser/browser_util.h"
#include "brave/components/brave_ads/core/internal/common/unittest/unittest_base.h"

// npm run test -- brave_unit_tests --filter=BraveAds*

namespace brave_ads {

class BraveAdsVersionNumberUserDataTest : public UnitTestBase {};

TEST_F(BraveAdsVersionNumberUserDataTest,
       BuildVersionNumberUserDataForRewardsUser) {
  // Arrange

  // Act

  // Assert
  const std::string expected_json = base::ReplaceStringPlaceholders(
      R"({"versionNumber":"$1"})", {GetBrowserVersionNumber()}, nullptr);
  EXPECT_EQ(base::test::ParseJsonDict(expected_json),
            BuildVersionNumberUserData());
}

TEST_F(BraveAdsVersionNumberUserDataTest,
       BuildVersionNumberUserDataForNonRewardsUser) {
  // Arrange
  DisableBraveRewardsForTesting();

  // Act

  // Assert
  EXPECT_TRUE(BuildVersionNumberUserData().empty());
}

}  // namespace brave_ads
