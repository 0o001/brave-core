/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_CHROMIUM_SRC_COMPONENTS_SUPERVISED_USER_CORE_BROWSER_SUPERVISED_USER_CONTENT_SETTINGS_PROVIDER_H_
#define BRAVE_CHROMIUM_SRC_COMPONENTS_SUPERVISED_USER_CORE_BROWSER_SUPERVISED_USER_CONTENT_SETTINGS_PROVIDER_H_

#include "brave/components/content_settings/core/browser/brave_global_value_map.h"

#define GlobalValueMap BraveGlobalValueMap
#include "src/components/supervised_user/core/browser/supervised_user_content_settings_provider.h"  // IWYU pragma: export
#undef GlobalValueMap

#endif  // BRAVE_CHROMIUM_SRC_COMPONENTS_SUPERVISED_USER_CORE_BROWSER_SUPERVISED_USER_CONTENT_SETTINGS_PROVIDER_H_
