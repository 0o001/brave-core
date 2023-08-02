/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_ads/core/internal/ads/ad_events/notification_ads/notification_ad_event_served.h"

#include <utility>

#include "brave/components/brave_ads/core/internal/ads/ad_events/ad_events.h"
#include "brave/components/brave_ads/core/public/confirmation_type.h"
#include "brave/components/brave_ads/core/public/notification_ad_info.h"

namespace brave_ads {

void NotificationAdEventServed::FireEvent(const NotificationAdInfo& ad,
                                          ResultCallback callback) {
  LogAdEvent(ad, ConfirmationType::kServed, std::move(callback));
}

}  // namespace brave_ads
