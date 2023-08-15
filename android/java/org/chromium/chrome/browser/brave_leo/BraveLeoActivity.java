/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.brave_leo;

import static androidx.browser.customtabs.CustomTabsIntent.COLOR_SCHEME_DARK;
import static androidx.browser.customtabs.CustomTabsIntent.COLOR_SCHEME_LIGHT;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.net.Uri;
import android.provider.Browser;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewParent;

import androidx.browser.customtabs.CustomTabsIntent;
import androidx.browser.customtabs.CustomTabsSessionToken;

import org.chromium.base.CommandLine;
import org.chromium.base.IntentUtils;
import org.chromium.base.Log;
import org.chromium.chrome.browser.IntentHandler;
import org.chromium.chrome.browser.LaunchIntentDispatcher;
import org.chromium.chrome.browser.app.metrics.LaunchCauseMetrics;
import org.chromium.chrome.browser.browserservices.intents.BrowserServicesIntentDataProvider;
import org.chromium.chrome.browser.compositor.CompositorViewHolder;
import org.chromium.chrome.browser.customtabs.BaseCustomTabActivity;
import org.chromium.chrome.browser.customtabs.CustomTabActivity;
import org.chromium.chrome.browser.customtabs.CustomTabIntentDataProvider;
import org.chromium.chrome.browser.customtabs.CustomTabLaunchCauseMetrics;
import org.chromium.chrome.browser.customtabs.IncognitoCustomTabIntentDataProvider;
import org.chromium.chrome.browser.customtabs.features.CustomTabNavigationBarController;
import org.chromium.chrome.browser.flags.ChromeSwitches;
import org.chromium.chrome.browser.webapps.WebappLaunchCauseMetrics;
import org.chromium.components.embedder_support.util.UrlConstants;
import org.chromium.ui.util.ColorUtils;

public class BraveLeoActivity extends /*CustomTabActivity*/BaseCustomTabActivity {

    @Override
    public void performPostInflationStartup() {
        super.performPostInflationStartup();

        CompositorViewHolder compositorViewHolder = getCompositorViewHolderSupplier().get();
        if (compositorViewHolder == null || compositorViewHolder.getCompositorView() == null) return;

        ViewParent compositor =  compositorViewHolder.getCompositorView().getParent();
        if (compositor instanceof ViewGroup) {
            //setTransparentBackground((View)compositor);
            //((View)compositor).setPadding(0, 500, 10, 500);
        }
        //getWindow().setBackgroundDrawable(new ColorDrawable(Color.TRANSPARENT));
    }

    @Override
    protected LaunchCauseMetrics createLaunchCauseMetrics() {
        return new WebappLaunchCauseMetrics(this, null);
    }

    private void setTransparentBackground(View v) {
        if (v == null) return;
        v.setAlpha(0f);
        ViewParent parent = v.getParent();
        if (parent instanceof View) {
            setTransparentBackground((View)parent);
        }
    }

    public static void showPage(Context context, String url) {
        Intent intent = new Intent();
        intent.setAction(Intent.ACTION_VIEW);
        intent.setClassName(context, BraveLeoActivity.class.getName());
        intent.addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP |
                Intent.FLAG_ACTIVITY_CLEAR_TOP);
        intent.putExtra(CustomTabsIntent.EXTRA_TITLE_VISIBILITY_STATE,
                CustomTabsIntent.SHOW_PAGE_TITLE);
        intent.putExtra(CustomTabsIntent.EXTRA_COLOR_SCHEME, ColorUtils.inNightMode(context) ? COLOR_SCHEME_DARK
                : COLOR_SCHEME_LIGHT);
        intent.setData(Uri.parse(url));
        intent.setPackage(context.getPackageName());
        intent.putExtra(CustomTabIntentDataProvider.EXTRA_UI_TYPE, BrowserServicesIntentDataProvider.CustomTabsUiType.INFO_PAGE);
        intent.putExtra(Browser.EXTRA_APPLICATION_ID, context.getPackageName());
        if (!(context instanceof Activity)) intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        IntentUtils.addTrustedIntentExtras(intent);

        context.startActivity(intent);
    }

    @Override
    protected BrowserServicesIntentDataProvider buildIntentDataProvider(Intent intent, int colorScheme) {
        if (IncognitoCustomTabIntentDataProvider.isValidIncognitoIntent(intent)) {
            return new IncognitoCustomTabIntentDataProvider(intent, this, colorScheme);
        }
        return new CustomTabIntentDataProvider(intent, this, colorScheme);
    }
}
