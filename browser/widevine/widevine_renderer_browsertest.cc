/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <memory>

#include "base/memory/raw_ptr.h"
#include "base/path_service.h"
#include "brave/app/brave_main_delegate.h"
#include "brave/browser/brave_content_browser_client.h"
#include "brave/browser/brave_drm_tab_helper.h"
#include "brave/browser/widevine/constants.h"
#include "brave/browser/widevine/widevine_utils.h"
#include "brave/components/constants/brave_paths.h"
#include "brave/components/constants/pref_names.h"
#include "brave/renderer/brave_content_renderer_client.h"
#include "build/build_config.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/test/base/chrome_test_utils.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/navigation_controller.h"
#include "content/public/browser/web_contents.h"
#include "content/public/common/content_client.h"
#include "content/public/renderer/content_renderer_client.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/browser_test_utils.h"
#include "content/public/test/test_utils.h"
#include "media/base/key_systems.h"
#include "media/base/key_system_info.h"
#include "net/dns/mock_host_resolver.h"
#include "content/browser/media/cdm_registry_impl.h"
#include "third_party/widevine/cdm/widevine_cdm_common.h"
#include "ui/base/page_transition_types.h"
#include "url/gurl.h"

#if BUILDFLAG(IS_ANDROID)
#include "chrome/test/base/android/android_browser_test.h"
#else
#include "chrome/test/base/in_process_browser_test.h"
#endif

const char kEmbeddedTestServerDirectory[] = "widevine";

class BraveWidevineRendererBrowserTest : public PlatformBrowserTest {
 public:
  BraveWidevineRendererBrowserTest() = default;

 protected:
  void SetUpOnMainThread() override {
    PlatformBrowserTest::SetUpOnMainThread();
    EnableWidevineCdm();

    content_client_ = std::make_unique<ChromeContentClient>();
    content::SetContentClient(content_client_.get());
    content_renderer_client_ = std::make_unique<BraveContentRendererClient>();
    content::SetRendererClientForTesting(content_renderer_client_.get());
    browser_content_client_ = std::make_unique<BraveContentBrowserClient>();
    content::SetBrowserClientForTesting(browser_content_client_.get());
    
    // HTTP resolver
    host_resolver()->AddRule("*", "127.0.0.1");
    brave::RegisterPathProvider();
    base::FilePath test_data_dir;
    base::PathService::Get(brave::DIR_TEST_DATA, &test_data_dir);
    test_data_dir = test_data_dir.AppendASCII(kEmbeddedTestServerDirectory);
    https_server_ = std::make_unique<net::EmbeddedTestServer>(
        net::test_server::EmbeddedTestServer::TYPE_HTTPS);
    https_server_->SetSSLConfig(net::EmbeddedTestServer::CERT_TEST_NAMES);
    https_server_->ServeFilesFromDirectory(test_data_dir);
    ASSERT_TRUE(https_server_->Start());

    request_test_url_ = https_server_->GetURL("a.test", "/widevine_request_test.html");
    cb_true_ = base::BindRepeating(&BraveWidevineRendererBrowserTest::AssertWidevineExist,
                          weak_factory_.GetWeakPtr());
  }

  void TearDownOnMainThread() override {
    weak_factory_.InvalidateWeakPtrs();
    PlatformBrowserTest::TearDownOnMainThread();
  }

  const GURL& request_test_url() { return request_test_url_; }

  content::WebContents* GetActiveWebContents() {
    return chrome_test_utils::GetActiveWebContents(this);
  }

  BraveDrmTabHelper* GetBraveDrmTabHelper() {
    return BraveDrmTabHelper::FromWebContents(GetActiveWebContents());
  }

  // content::ContentMainDelegate* GetOptionalContentMainDelegateOverride() override {
  //   return new BraveMainDelegate(base::TimeTicks::Now());
  // }

  bool IsWidevineExist(media::KeySystemInfos* key_systems) {
    for (auto const& key_system : (*key_systems)) {
      if (key_system->GetBaseKeySystemName() == kWidevineKeySystem) return true;
    }
    return false;
  }

  void AssertWidevineExist(media::KeySystemInfos key_systems) {
    ASSERT_TRUE(IsWidevineExist(&key_systems));
  }

  media::GetSupportedKeySystemsCB AssertWidevineExistCB() {
    return cb_true_;
  }

  private:
    GURL request_test_url_;
    std::unique_ptr<net::EmbeddedTestServer> https_server_;
    std::unique_ptr<ChromeContentClient> content_client_;
    std::unique_ptr<BraveContentBrowserClient> browser_content_client_;
    std::unique_ptr<BraveContentRendererClient> content_renderer_client_;
    media::GetSupportedKeySystemsCB cb_true_;
    base::WeakPtrFactory<BraveWidevineRendererBrowserTest> weak_factory_{this};
};

IN_PROC_BROWSER_TEST_F(BraveWidevineRendererBrowserTest, RemoveWidevineTest) {
  // auto* drm_tab_helper = GetBraveDrmTabHelper();

  // Check enable state
  // ASSERT_TRUE(media::KeySystems::GetInstance()->IsSupportedKeySystem(kWidevineKeySystem));
  EXPECT_TRUE(IsWidevineOptedIn());

  // GURL url = GURL("https://www.google.com/");
  // content::NavigationController::LoadURLParams params(url);
  // params.transition_type = ui::PageTransitionFromInt(
  //     ui::PAGE_TRANSITION_TYPED | ui::PAGE_TRANSITION_FROM_ADDRESS_BAR);
  // 
  // auto* web_contents = GetActiveWebContents();
  // 
  // web_contents->GetController().LoadURLWithParams(params);
  // web_contents->GetOutermostWebContents()->Focus();
  // EXPECT_TRUE(WaitForLoadStop(web_contents));
  // EXPECT_TRUE(web_contents->GetLastCommittedURL() == url)
  //     << "Expected URL " << url << " but observed "
  //     << web_contents->GetLastCommittedURL();
  
  EXPECT_TRUE(content::NavigateToURL(GetActiveWebContents(),
                                     request_test_url()));

  content::RunAllTasksUntilIdle();
  // content::GetContentClientForTesting()->renderer()->GetSupportedKeySystems(AssertWidevineExistCB());
  // media::GetMediaClient()->GetSupportedKeySystems(AssertWidevineExistCB());
  ASSERT_TRUE(media::KeySystems::GetInstance()->IsSupportedKeySystem(kWidevineKeySystem));
  // content::CdmRegistryImpl::GetInstance()->

  // Check permission is requested again after new navigation.
  // EXPECT_TRUE(content::NavigateToURL(GetActiveWebContents(),
  //                                    GURL("chrome://newtab/")));
  // drm_tab_helper->OnWidevineKeySystemAccessRequest();
  // content::RunAllTasksUntilIdle();
  // EXPECT_TRUE(observer.bubble_added_);
}
