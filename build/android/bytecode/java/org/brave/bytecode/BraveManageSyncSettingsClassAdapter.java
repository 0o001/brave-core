/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.brave.bytecode;

import org.objectweb.asm.ClassVisitor;

/**
 * Adapter, used to make some fields from upstream's ManageSyncSettings be public
 */
public class BraveManageSyncSettingsClassAdapter extends BraveClassVisitor {
    static String sManageSyncSettingsClassName =
            "org/chromium/chrome/browser/sync/settings/ManageSyncSettings";
    static String sBraveManageSyncSettingsClassName =
            "org/chromium/chrome/browser/sync/settings/BraveManageSyncSettings";

    BraveManageSyncSettingsClassAdapter(ClassVisitor visitor) {
        super(visitor);

        deleteField(sBraveManageSyncSettingsClassName, "mGoogleActivityControls");
        makeProtectedField(sManageSyncSettingsClassName, "mGoogleActivityControls");

        deleteField(sBraveManageSyncSettingsClassName, "mSyncEncryption");
        makeProtectedField(sManageSyncSettingsClassName, "mSyncEncryption");

        deleteField(sBraveManageSyncSettingsClassName, "mSyncPaymentsIntegration");
        makeProtectedField(sManageSyncSettingsClassName, "mSyncPaymentsIntegration");

        deleteField(sBraveManageSyncSettingsClassName, "mSyncEverything");
        makeProtectedField(sManageSyncSettingsClassName, "mSyncEverything");
    }
}
