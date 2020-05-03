/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/common/brave_switches.h"
#include "chrome/common/chrome_features.h"
#include "chrome/test/base/in_process_browser_test.h"

class BraveMainDelegateRuntimeFlagsBrowserTest : public InProcessBrowserTest {
 public:
  BraveMainDelegateRuntimeFlagsBrowserTest() {}
  ~BraveMainDelegateRuntimeFlagsBrowserTest() override {}

  void SetUpCommandLine(base::CommandLine* command_line) override {
    command_line->AppendSwitch(switches::kDisableDnsOverHttps);
  }
};

IN_PROC_BROWSER_TEST_F(BraveMainDelegateRuntimeFlagsBrowserTest,
                       DisableDnsOverHTTPS) {
  EXPECT_FALSE(base::FeatureList::IsEnabled(features::kDnsOverHttps));
}
