/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_ads/core/internal/account/utility/redeem_unblinded_token/redeem_unblinded_token_util.h"

#include "base/strings/stringprintf.h"
#include "brave/components/brave_ads/core/ad_type.h"
#include "brave/components/brave_ads/core/internal/server/url/hosts/server_host_util.h"

namespace brave_ads {

std::string GetAnonymousHost(const AdType& ad_type) {
  return base::StringPrintf("%s%s",
                            ad_type == AdType::kSearchResultAd ? "search." : "",
                            server::GetAnonymousHost().c_str());
}

}  // namespace brave_ads
