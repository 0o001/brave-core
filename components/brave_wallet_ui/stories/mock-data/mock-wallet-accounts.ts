// Copyright (c) 2022 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.
import { BraveWallet, WalletAccountType } from '../../constants/types'

export const mockAccounts: WalletAccountType[] = [
  {
    name: 'Account 1',
    address: '0x7d66c9ddAED3115d93Bd1790332f3Cd06Cf52B14',
    nativeBalanceRegistry: {
      '0x1': '311780000000000000'
    },
    tokenBalanceRegistry: {},
    accountId: {
      coin: BraveWallet.CoinType.ETH,
      keyringId: BraveWallet.KeyringId.kDefault,
      kind: BraveWallet.AccountKind.kDerived,
      address: '0x7d66c9ddAED3115d93Bd1790332f3Cd06Cf52B14',
      uniqueKey: '1'
    },
    hardware: undefined
  },
  {
    name: 'Account 2',
    address: '0x73A29A1da97149722eB09c526E4eAd698895bDCf',
    nativeBalanceRegistry: {
      '0x1': '311780000000000000'
    },
    tokenBalanceRegistry: {},
    accountId: {
      coin: BraveWallet.CoinType.ETH,
      keyringId: BraveWallet.KeyringId.kDefault,
      kind: BraveWallet.AccountKind.kDerived,
      address: '0x73A29A1da97149722eB09c526E4eAd698895bDCf',
      uniqueKey: '2',
    },
    hardware: undefined
  },
  {
    name: 'Account 3',
    address: '0x3f29A1da97149722eB09c526E4eAd698895b426',
    nativeBalanceRegistry: {
      '0x1': '311780000000000000'
    },
    tokenBalanceRegistry: {},
    accountId: {
      coin: BraveWallet.CoinType.ETH,
      keyringId: BraveWallet.KeyringId.kDefault,
      kind: BraveWallet.AccountKind.kDerived,
      address: '0x3f29A1da97149722eB09c526E4eAd698895b426',
      uniqueKey: '3',
    },
    hardware: undefined
  }
]

export const mockedTransactionAccounts: WalletAccountType[] = [
  {
    name: 'Account 1',
    address: '1',
    nativeBalanceRegistry: {
      '0x1': '311780000000000000'
    },
    tokenBalanceRegistry: {},
    accountId: {
      coin: BraveWallet.CoinType.ETH,
      keyringId: BraveWallet.KeyringId.kDefault,
      kind: BraveWallet.AccountKind.kDerived,
      address: '1',
      uniqueKey: '1',
    },
    hardware: undefined
  }
]
