/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.brave_leo;

import android.annotation.SuppressLint;
import android.app.Dialog;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewParent;

import androidx.annotation.NonNull;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

import com.google.android.material.bottomsheet.BottomSheetDialogFragment;

import org.chromium.base.Log;
import org.chromium.chrome.browser.customtabs.CustomTabActivity;

public class BraveLeoBottomSheetDialogFragment extends BottomSheetDialogFragment {
    private static final String TAG = "BraveLeoF";
    public static final String TAG_FRAGMENT = BraveLeoBottomSheetDialogFragment.class.getName();

    @Override
    public void show(FragmentManager manager, String tag) {
        try {
            BraveLeoBottomSheetDialogFragment fragment =
                    (BraveLeoBottomSheetDialogFragment) manager.findFragmentByTag(
                            BraveLeoBottomSheetDialogFragment.TAG_FRAGMENT);
            FragmentTransaction transaction = manager.beginTransaction();
            if (fragment != null) {
                transaction.remove(fragment);
            }
            transaction.add(this, tag);
            transaction.commitAllowingStateLoss();
        } catch (IllegalStateException e) {
            Log.e(TAG, e.getMessage());
        }
    }

    @Override
    public void setupDialog(@NonNull Dialog dialog, int style) {
        super.setupDialog(dialog, style);
        @SuppressLint("InflateParams") final View view =
                LayoutInflater.from(getContext()).inflate(org.chromium.chrome.R.layout.brave_leo_bottom_sheet, null);
        dialog.setContentView(view);
        ViewParent parent = view.getParent();
        ((View) parent).getLayoutParams().height = ViewGroup.LayoutParams.WRAP_CONTENT;

        CustomTabActivity.showInfoPage(getActivity(), "chrome-untrusted://chat");
    }
}
