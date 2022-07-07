package org.chromium.chrome.browser.crypto_wallet.fragments;

import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.recyclerview.widget.RecyclerView;

import com.google.android.material.bottomsheet.BottomSheetDialogFragment;

import org.chromium.chrome.R;
import org.chromium.chrome.browser.app.BraveActivity;
import org.chromium.chrome.browser.app.domain.WalletModel;
import org.chromium.chrome.browser.crypto_wallet.activities.AddAccountActivity;
import org.chromium.chrome.browser.crypto_wallet.adapters.CreateAccountAdapter;
import org.chromium.chrome.browser.crypto_wallet.model.CryptoAccountTypeInfo;

import java.util.ArrayList;
import java.util.List;

public class CreateAccountBottomSheetFragment extends BottomSheetDialogFragment
        implements CreateAccountAdapter.OnCreateAccountClickListener {
    public static final String TAG = "CreateAccountBottomSheetFragment";
    private View rootView;
    private WalletModel mWalletModel;
    private List<CryptoAccountTypeInfo> mSupportedCryptoAccounts;
    private RecyclerView mRvAccounts;
    private CreateAccountAdapter mCreateAccountAdapter;

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mSupportedCryptoAccounts = new ArrayList<>();
    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container,
            @Nullable Bundle savedInstanceState) {
        rootView = inflater.inflate(R.layout.fragment_create_account, container, false);
        return rootView;
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        mSupportedCryptoAccounts.clear();
        BraveActivity activity = BraveActivity.getBraveActivity();
        if (activity != null) {
            mWalletModel = activity.getWalletModel();
            mSupportedCryptoAccounts =
                    mWalletModel.getCryptoModel().getSupportedCryptoAccountTypes();
        }
        mRvAccounts = view.findViewById(R.id.fragment_create_account_rv);

        mCreateAccountAdapter =
                new CreateAccountAdapter(requireContext(), mSupportedCryptoAccounts);
        mRvAccounts.setAdapter(mCreateAccountAdapter);
        mCreateAccountAdapter.setOnAccountItemSelected(this);
    }

    @Override
    public void onAccountClick(CryptoAccountTypeInfo cryptoAccountTypeInfo) {
        Intent addAccountActivityIntent = new Intent(getActivity(), AddAccountActivity.class);
        addAccountActivityIntent.putExtra(AddAccountActivity.ACCOUNT, cryptoAccountTypeInfo);
        startActivity(addAccountActivityIntent);
        dismiss();
    }
}