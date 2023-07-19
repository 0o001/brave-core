/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/extensions/api/brave_education_api.h"

#include "brave/browser/ui/tabs/brave_tab_prefs.h"
#include "brave/common/extensions/api/brave_education.h"
#include "chrome/browser/extensions/extension_tab_util.h"
#include "chrome/browser/profiles/profile.h"
#include "components/prefs/pref_service.h"

namespace extensions {
namespace api {

ExtensionFunction::ResponseAction
BraveEducationOpenSettingsFunction::Run() {
  auto params = brave_education::OpenSettings::Params::Create(args());
  EXTENSION_FUNCTION_VALIDATE(params);
  return RespondNow([&] {
    if (ExtensionTabUtil::IsTabStripEditable()) {
      ExtensionTabUtil::OpenTabParams options;
      options.url = params->url;
      auto result = ExtensionTabUtil::OpenTab(this, options, user_gesture());
      if (!result.has_value()) {
        return Error(result.error());
      }
    }
    return NoArguments();
  }());
}

ExtensionFunction::ResponseAction
BraveEducationEnableVerticalTabsFunction::Run() {
  return RespondNow([&] {
    auto* profile = Profile::FromBrowserContext(browser_context());
    profile->GetPrefs()->SetBoolean(brave_tabs::kVerticalTabsEnabled, true);
    return NoArguments();
  }());
}

}  // namespace api
}  // namespace extensions
