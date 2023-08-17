/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.base.library_loader;

import android.annotation.SuppressLint;
import android.content.pm.ApplicationInfo;
import android.os.Build;

import androidx.annotation.VisibleForTesting;

import org.chromium.base.BraveReflectionUtil;
import org.chromium.base.BundleUtils;
import org.chromium.build.NativeLibraries;

/**
 * Class to override upstream's LibraryLoader.loadWithSystemLinkerAlreadyLocked
 * See also BraveLibraryLoaderClassAdapter
 */
public class BraveLibraryLoader extends LibraryLoader {
    @VisibleForTesting
    public BraveLibraryLoader() {
        super();
    }

    public static LibraryLoader sInstance = new BraveLibraryLoader();
    public static LibraryLoader getInstance() {
        return sInstance;
    }

    public void loadWithSystemLinkerAlreadyLocked(ApplicationInfo appInfo, boolean inZygote) {
        try {
            callChromiumLoadWithSystemLinkerAlreadyLocked(appInfo, inZygote);
        } catch (UnsatisfiedLinkError ex) {
            loadLibsfromSplits();
        }
    }

    private void callChromiumLoadWithSystemLinkerAlreadyLocked(
            ApplicationInfo appInfo, boolean inZygote) {
        // This is invoked, but got into endless recursion, because down here
        // BraveLibraryLoader.loadWithSystemLinkerAlreadyLocked is invoked instead of
        // LibraryLoader.loadWithSystemLinkerAlreadyLocked
        BraveReflectionUtil.InvokeMethod(LibraryLoader.class, (LibraryLoader) this,
                "loadWithSystemLinkerAlreadyLocked", ApplicationInfo.class, appInfo, boolean.class,
                inZygote);

        // This would work if the super method would be public:
        // public void loadWithSystemLinkerAlreadyLocked
        // super.loadWithSystemLinkerAlreadyLocked(appInfo, inZygote);
    }

    @SuppressLint({"UnsafeDynamicallyLoadedCode"})
    private void loadLibsfromSplits() {
        String splitName = getSplitNameByCurrentAbi();
        for (String library : NativeLibraries.LIBRARIES) {
            System.load(BundleUtils.getNativeLibraryPath(library, splitName));
        }
    }

    private String getSplitNameByCurrentAbi() {
        assert Build.SUPPORTED_ABIS.length > 0;
        String currentAbi = Build.SUPPORTED_ABIS[0];
        switch (currentAbi) {
            case "arm64-v8a":
                return "config.arm64_v8a";
            case "armeabi-v7a":
                return "config.armeabi_v7a";
            case "x86_64":
                return "config.x86_64";
            case "x86":
                return "config.x86";
            default:
                assert false;
                return "config.arm64_v8a";
        }
    }
}
