/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_wallet/browser/eth_data_parser.h"

#include <optional>
#include <string>
#include <vector>

#include "brave/components/brave_wallet/browser/brave_wallet_utils.h"
#include "brave/components/brave_wallet/common/hex_utils.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace brave_wallet {

namespace {
void TestGetTransactionInfoFromData(const std::vector<uint8_t>& data,
                                    mojom::TransactionType expected_tx_type,
                                    std::vector<std::string> expected_tx_params,
                                    std::vector<std::string> expected_tx_args) {
  mojom::TransactionType tx_type;
  std::vector<std::string> tx_params;
  std::vector<std::string> tx_args;

  auto result = GetTransactionInfoFromData(data);
  ASSERT_NE(result, std::nullopt);
  std::tie(tx_type, tx_params, tx_args) = *result;
  ASSERT_EQ(tx_type, expected_tx_type);
  ASSERT_EQ(tx_params.size(), expected_tx_params.size());
  ASSERT_EQ(tx_args.size(), expected_tx_args.size());

  for (int i = 0; i < static_cast<int>(tx_params.size()); i++) {
    ASSERT_EQ(tx_params[i], expected_tx_params[i]);
  }

  for (int i = 0; i < static_cast<int>(tx_args.size()); i++) {
    ASSERT_EQ(tx_args[i], expected_tx_args[i]);
  }
}
}  // namespace

TEST(EthDataParser, GetTransactionInfoFromDataTransfer) {
  mojom::TransactionType tx_type;
  std::vector<std::string> tx_params;
  std::vector<std::string> tx_args;
  std::vector<uint8_t> data;

  // OK: well-formed ERC20Transfer
  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0xa9059cbb"
      "000000000000000000000000BFb30a082f650C2A15D0632f0e87bE4F8e64460f"
      "0000000000000000000000000000000000000000000000003fffffffffffffff",
      &data));
  auto tx_info = GetTransactionInfoFromData(data);
  ASSERT_NE(tx_info, std::nullopt);
  std::tie(tx_type, tx_params, tx_args) = *tx_info;
  ASSERT_EQ(tx_type, mojom::TransactionType::ERC20Transfer);
  ASSERT_EQ(tx_params.size(), 2UL);
  EXPECT_EQ(tx_params[0], "address");
  EXPECT_EQ(tx_params[1], "uint256");
  ASSERT_EQ(tx_args.size(), 2UL);
  EXPECT_EQ(tx_args[0], "0xbfb30a082f650c2a15d0632f0e87be4f8e64460f");
  EXPECT_EQ(tx_args[1], "0x3fffffffffffffff");

  // KO: missing a byte for the last param
  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0xa9059cbb"
      "000000000000000000000000BFb30a082f650C2A15D0632f0e87bE4F8e64460f"
      "0000000000000000000000000000000000000000000000003fffffffffffff",
      &data));
  EXPECT_FALSE(GetTransactionInfoFromData(data));

  // KO: missing the entire last param
  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0xa9059cbb"
      "000000000000000000000000BFb30a082f650C2A15D0632f0e87bE4F8e64460f",
      &data));
  EXPECT_FALSE(GetTransactionInfoFromData(data));

  // KO: no params
  ASSERT_TRUE(PrefixedHexStringToBytes("0xa9059cbb", &data));
  EXPECT_FALSE(GetTransactionInfoFromData(data));

  // OK: extra data
  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0xa9059cbb"
      "000000000000000000000000BFb30a082f650C2A15D0632f0e87bE4F8e64460f"
      "0000000000000000000000000000000000000000000000003fffffffffffffff"
      "00",
      &data));
  tx_info = GetTransactionInfoFromData(data);
  ASSERT_NE(tx_info, std::nullopt);
  std::tie(tx_type, tx_params, tx_args) = *tx_info;
  ASSERT_EQ(tx_type, mojom::TransactionType::ERC20Transfer);
  ASSERT_EQ(tx_params.size(), 2UL);
  EXPECT_EQ(tx_params[0], "address");
  EXPECT_EQ(tx_params[1], "uint256");
  ASSERT_EQ(tx_args.size(), 2UL);
  EXPECT_EQ(tx_args[0], "0xbfb30a082f650c2a15d0632f0e87be4f8e64460f");
  EXPECT_EQ(tx_args[1], "0x3fffffffffffffff");
}

TEST(EthDataParser, GetTransactionInfoFromDataApprove) {
  mojom::TransactionType tx_type;
  std::vector<std::string> tx_params;
  std::vector<std::string> tx_args;
  std::vector<uint8_t> data;

  // OK: well-formed ERC20Approve
  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0x095ea7b3"
      "000000000000000000000000BFb30a082f650C2A15D0632f0e87bE4F8e64460f"
      "0000000000000000000000000000000000000000000000003fffffffffffffff",
      &data));
  auto tx_info = GetTransactionInfoFromData(data);
  ASSERT_NE(tx_info, std::nullopt);
  std::tie(tx_type, tx_params, tx_args) = *tx_info;
  ASSERT_EQ(tx_type, mojom::TransactionType::ERC20Approve);
  ASSERT_EQ(tx_params.size(), 2UL);
  EXPECT_EQ(tx_params[0], "address");
  EXPECT_EQ(tx_params[1], "uint256");
  ASSERT_EQ(tx_args.size(), 2UL);
  EXPECT_EQ(tx_args[0], "0xbfb30a082f650c2a15d0632f0e87be4f8e64460f");
  EXPECT_EQ(tx_args[1], "0x3fffffffffffffff");

  // OK: function case doesn't matter
  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0x095EA7b3"
      "000000000000000000000000BFb30a082f650C2A15D0632f0e87bE4F8e64460f"
      "0000000000000000000000000000000000000000000000003fffffffffffffff",
      &data));
  tx_info = GetTransactionInfoFromData(data);
  ASSERT_NE(tx_info, std::nullopt);
  std::tie(tx_type, tx_params, tx_args) = *tx_info;
  ASSERT_EQ(tx_type, mojom::TransactionType::ERC20Approve);
  ASSERT_EQ(tx_params.size(), 2UL);
  EXPECT_EQ(tx_params[0], "address");
  EXPECT_EQ(tx_params[1], "uint256");
  ASSERT_EQ(tx_args.size(), 2UL);
  EXPECT_EQ(tx_args[0], "0xbfb30a082f650c2a15d0632f0e87be4f8e64460f");
  EXPECT_EQ(tx_args[1], "0x3fffffffffffffff");

  // KO: missing a byte for the last param
  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0x095ea7b3"
      "000000000000000000000000BFb30a082f650C2A15D0632f0e87bE4F8e64460f"
      "0000000000000000000000000000000000000000000000003fffffffffffff",
      &data));
  EXPECT_FALSE(GetTransactionInfoFromData(data));

  // KO: missing the entire last param
  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0x095ea7b3"
      "000000000000000000000000BFb30a082f650C2A15D0632f0e87bE4F8e64460f",
      &data));
  EXPECT_FALSE(GetTransactionInfoFromData(data));

  // KO: no params
  ASSERT_TRUE(PrefixedHexStringToBytes("0x095ea7b3", &data));
  EXPECT_FALSE(GetTransactionInfoFromData(data));

  // OK: extra data
  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0x095ea7b3"
      "000000000000000000000000BFb30a082f650C2A15D0632f0e87bE4F8e64460f"
      "0000000000000000000000000000000000000000000000003fffffffffffffff"
      "00",
      &data));
  tx_info = GetTransactionInfoFromData(data);
  ASSERT_NE(tx_info, std::nullopt);
  std::tie(tx_type, tx_params, tx_args) = *tx_info;
  ASSERT_EQ(tx_type, mojom::TransactionType::ERC20Approve);
  ASSERT_EQ(tx_params.size(), 2UL);
  EXPECT_EQ(tx_params[0], "address");
  EXPECT_EQ(tx_params[1], "uint256");
  ASSERT_EQ(tx_args.size(), 2UL);
  EXPECT_EQ(tx_args[0], "0xbfb30a082f650c2a15d0632f0e87be4f8e64460f");
  EXPECT_EQ(tx_args[1], "0x3fffffffffffffff");
}

TEST(EthDataParser, GetTransactionInfoFromDataETHSend) {
  mojom::TransactionType tx_type;
  std::vector<std::string> tx_params;
  std::vector<std::string> tx_args;

  std::vector<uint8_t> data;
  ASSERT_TRUE(PrefixedHexStringToBytes("0x0", &data));
  auto tx_info = GetTransactionInfoFromData(data);
  ASSERT_NE(tx_info, std::nullopt);
  std::tie(tx_type, tx_params, tx_args) = *tx_info;

  EXPECT_EQ(tx_type, mojom::TransactionType::ETHSend);
  ASSERT_EQ(tx_params.size(), 0UL);
  ASSERT_EQ(tx_args.size(), 0UL);

  tx_info = GetTransactionInfoFromData({});
  ASSERT_NE(tx_info, std::nullopt);
  std::tie(tx_type, tx_params, tx_args) = *tx_info;
  EXPECT_EQ(tx_type, mojom::TransactionType::ETHSend);
  ASSERT_EQ(tx_params.size(), 0UL);
  ASSERT_EQ(tx_args.size(), 0UL);
}

TEST(EthDataParser, GetTransactionInfoFromDataERC721TransferFrom) {
  mojom::TransactionType tx_type;
  std::vector<std::string> tx_params;
  std::vector<std::string> tx_args;
  std::vector<uint8_t> data;

  // OK: well-formed ERC721TransferFrom
  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0x23b872dd"
      "000000000000000000000000BFb30a082f650C2A15D0632f0e87bE4F8e64460f"
      "000000000000000000000000BFb30a082f650C2A15D0632f0e87bE4F8e64460a"
      "000000000000000000000000000000000000000000000000000000000000000f",
      &data));
  auto tx_info = GetTransactionInfoFromData(data);
  ASSERT_NE(tx_info, std::nullopt);
  std::tie(tx_type, tx_params, tx_args) = *tx_info;
  ASSERT_EQ(tx_type, mojom::TransactionType::ERC721TransferFrom);
  ASSERT_EQ(tx_params.size(), 3UL);
  EXPECT_EQ(tx_params[0], "address");
  EXPECT_EQ(tx_params[1], "address");
  EXPECT_EQ(tx_params[2], "uint256");
  ASSERT_EQ(tx_args.size(), 3UL);
  EXPECT_EQ(tx_args[0], "0xbfb30a082f650c2a15d0632f0e87be4f8e64460f");
  EXPECT_EQ(tx_args[1], "0xbfb30a082f650c2a15d0632f0e87be4f8e64460a");
  EXPECT_EQ(tx_args[2], "0xf");

  // OK: well-formed ERC721SafeTransferFrom
  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0x42842e0e"
      "000000000000000000000000BFb30a082f650C2A15D0632f0e87bE4F8e64460f"
      "000000000000000000000000BFb30a082f650C2A15D0632f0e87bE4F8e64460a"
      "000000000000000000000000000000000000000000000000000000000000000f",
      &data));
  tx_info = GetTransactionInfoFromData(data);
  ASSERT_NE(tx_info, std::nullopt);
  std::tie(tx_type, tx_params, tx_args) = *tx_info;
  ASSERT_EQ(tx_type, mojom::TransactionType::ERC721SafeTransferFrom);
  ASSERT_EQ(tx_params.size(), 3UL);
  EXPECT_EQ(tx_params[0], "address");
  EXPECT_EQ(tx_params[1], "address");
  EXPECT_EQ(tx_params[2], "uint256");
  ASSERT_EQ(tx_args.size(), 3UL);
  EXPECT_EQ(tx_args[0], "0xbfb30a082f650c2a15d0632f0e87be4f8e64460f");
  EXPECT_EQ(tx_args[1], "0xbfb30a082f650c2a15d0632f0e87be4f8e64460a");
  EXPECT_EQ(tx_args[2], "0xf");

  // KO: missing a byte for the last param
  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0x23b872dd"
      "000000000000000000000000BFb30a082f650C2A15D0632f0e87bE4F8e64460f"
      "000000000000000000000000BFb30a082f650C2A15D0632f0e87bE4F8e64460a"
      "00000000000000000000000000000000000000000000000000000000000000",
      &data));
  EXPECT_FALSE(GetTransactionInfoFromData(data));

  // KO: missing the entire last param
  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0x23b872dd"
      "000000000000000000000000BFb30a082f650C2A15D0632f0e87bE4F8e64460f"
      "000000000000000000000000BFb30a082f650C2A15D0632f0e87bE4F8e64460a",
      &data));
  EXPECT_FALSE(GetTransactionInfoFromData(data));

  // KO: no params
  ASSERT_TRUE(PrefixedHexStringToBytes("0x23b872dd", &data));
  EXPECT_FALSE(GetTransactionInfoFromData(data));

  // OK: extra data
  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0x23b872dd"
      "000000000000000000000000BFb30a082f650C2A15D0632f0e87bE4F8e64460f"
      "000000000000000000000000BFb30a082f650C2A15D0632f0e87bE4F8e64460a"
      "000000000000000000000000000000000000000000000000000000000000000f"
      "00",
      &data));
  tx_info = GetTransactionInfoFromData(data);
  ASSERT_NE(tx_info, std::nullopt);
  std::tie(tx_type, tx_params, tx_args) = *tx_info;
  ASSERT_EQ(tx_type, mojom::TransactionType::ERC721TransferFrom);
  ASSERT_EQ(tx_params.size(), 3UL);
  EXPECT_EQ(tx_params[0], "address");
  EXPECT_EQ(tx_params[1], "address");
  EXPECT_EQ(tx_params[2], "uint256");
  ASSERT_EQ(tx_args.size(), 3UL);
  EXPECT_EQ(tx_args[0], "0xbfb30a082f650c2a15d0632f0e87be4f8e64460f");
  EXPECT_EQ(tx_args[1], "0xbfb30a082f650c2a15d0632f0e87be4f8e64460a");
  EXPECT_EQ(tx_args[2], "0xf");
}

TEST(EthDataParser, GetTransactionInfoFromDataERC1155SafeTransferFrom) {
  std::vector<uint8_t> data;
  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0xf242432a"  // function selector
      /*********************** HEAD (32x5 bytes) **********************/
      "00000000000000000000000016e4476c8fddc552e3b1c4b8b56261d85977fe52"
      "00000000000000000000000016e4476c8fddc552e3b1c4b8b56261d85977fe52"
      "0000000000000000000000000000000000000000000000000000000000000000"
      "0000000000000000000000000000000000000000000000000000000000000001"
      "00000000000000000000000000000000000000000000000000000000000000a0"

      /***************************** TAIL *****************************/
      "0000000000000000000000000000000000000000000000000000000000000000",
      &data));
  TestGetTransactionInfoFromData(
      data, mojom::TransactionType::ERC1155SafeTransferFrom,
      std::vector<std::string>{"address", "address", "uint256", "uint256",
                               "bytes"},
      std::vector<std::string>{"0x16e4476c8fddc552e3b1c4b8b56261d85977fe52",
                               "0x16e4476c8fddc552e3b1c4b8b56261d85977fe52",
                               "0x0", "0x1", "0x"});

  // Valid empty bytes with extra tail data
  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0xf242432a"  // function selector
      /*********************** HEAD (32x5 bytes) **********************/
      "00000000000000000000000016e4476c8fddc552e3b1c4b8b56261d85977fe52"
      "00000000000000000000000016e4476c8fddc552e3b1c4b8b56261d85977fe52"
      "0000000000000000000000000000000000000000000000000000000000000000"
      "0000000000000000000000000000000000000000000000000000000000000001"
      "00000000000000000000000000000000000000000000000000000000000000a0"

      /***************************** TAIL *****************************/
      "0000000000000000000000000000000000000000000000000000000000000000"
      "0000000000000000000000000000000000000000000000000000000000000000",
      &data));
  TestGetTransactionInfoFromData(
      data, mojom::TransactionType::ERC1155SafeTransferFrom,
      std::vector<std::string>{"address", "address", "uint256", "uint256",
                               "bytes"},
      std::vector<std::string>{"0x16e4476c8fddc552e3b1c4b8b56261d85977fe52",
                               "0x16e4476c8fddc552e3b1c4b8b56261d85977fe52",
                               "0x0", "0x1", "0x"});

  // Valid non-empty bytes
  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0xf242432a"  // function selector
      /*********************** HEAD (32x5 bytes) **********************/
      "00000000000000000000000016e4476c8fddc552e3b1c4b8b56261d85977fe52"
      "00000000000000000000000016e4476c8fddc552e3b1c4b8b56261d85977fe52"
      "0000000000000000000000000000000000000000000000000000000000000000"
      "0000000000000000000000000000000000000000000000000000000000000001"
      "00000000000000000000000000000000000000000000000000000000000000a0"

      /***************************** TAIL *****************************/
      "0000000000000000000000000000000000000000000000000000000000000010"
      "00000000000000000000000000000001"

      // extraneous calldata
      "00000000000000000000000000000000",
      &data));
  TestGetTransactionInfoFromData(
      data, mojom::TransactionType::ERC1155SafeTransferFrom,
      std::vector<std::string>{"address", "address", "uint256", "uint256",
                               "bytes"},
      std::vector<std::string>{"0x16e4476c8fddc552e3b1c4b8b56261d85977fe52",
                               "0x16e4476c8fddc552e3b1c4b8b56261d85977fe52",
                               "0x0", "0x1",
                               "0x00000000000000000000000000000001"});

  // Invalid non-empty bytes (length parameter too large)
  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0xf242432a"  // function selector
      /*********************** HEAD (32x5 bytes) **********************/
      "00000000000000000000000016e4476c8fddc552e3b1c4b8b56261d85977fe52"
      "00000000000000000000000016e4476c8fddc552e3b1c4b8b56261d85977fe52"
      "0000000000000000000000000000000000000000000000000000000000000000"
      "0000000000000000000000000000000000000000000000000000000000000001"
      "00000000000000000000000000000000000000000000000000000000000000a0"

      /***************************** TAIL *****************************/
      "0000000000000000000000000000000000000000000000000000000000000030"
      "0000000000000000000000000000000100000000000000000000000000000000",
      &data));
  EXPECT_FALSE(GetTransactionInfoFromData(data));

  // Invalid (missing length)
  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0xf242432a"  // function selector
      /*********************** HEAD (32x5 bytes) **********************/
      "00000000000000000000000016e4476c8fddc552e3b1c4b8b56261d85977fe52"
      "00000000000000000000000016e4476c8fddc552e3b1c4b8b56261d85977fe52"
      "0000000000000000000000000000000000000000000000000000000000000000"
      "0000000000000000000000000000000000000000000000000000000000000001"
      "00000000000000000000000000000000000000000000000000000000000000a0",
      &data));
  EXPECT_FALSE(GetTransactionInfoFromData(data));

  // Invalid (incorrect offset)
  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0xf242432a"  // function selector
      /*********************** HEAD (32x5 bytes) **********************/
      "00000000000000000000000016e4476c8fddc552e3b1c4b8b56261d85977fe52"
      "00000000000000000000000016e4476c8fddc552e3b1c4b8b56261d85977fe52"
      "0000000000000000000000000000000000000000000000000000000000000000"
      "0000000000000000000000000000000000000000000000000000000000000001"
      "0000000000000000000000000000000000000000000000000000000000000020"

      /***************************** TAIL *****************************/
      "0000000000000000000000000000000000000000000000000000000000000000",
      &data));
  EXPECT_FALSE(GetTransactionInfoFromData(data));

  // Invalid (no params)
  ASSERT_TRUE(PrefixedHexStringToBytes("0xf242432a", &data));
  EXPECT_FALSE(GetTransactionInfoFromData(data));
}

TEST(EthDataParser, GetTransactionInfoFromDataOther) {
  mojom::TransactionType tx_type;
  std::vector<std::string> tx_params;
  std::vector<std::string> tx_args;

  std::vector<uint8_t> data;

  // No function hash
  auto tx_info = GetTransactionInfoFromData(std::vector<uint8_t>{0x1});
  ASSERT_NE(tx_info, std::nullopt);
  std::tie(tx_type, tx_params, tx_args) = *tx_info;
  EXPECT_EQ(tx_type, mojom::TransactionType::Other);

  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0xaa0ffceb"
      "000000000000000000000000BFb30a082f650C2A15D0632f0e87bE4F8e64460f",
      &data));
  tx_info = GetTransactionInfoFromData(data);
  ASSERT_NE(tx_info, std::nullopt);
  std::tie(tx_type, tx_params, tx_args) = *tx_info;
  EXPECT_EQ(tx_type, mojom::TransactionType::Other);
}

TEST(EthDataParser, GetTransactionInfoFromDataSellEthForTokenToUniswapV3) {
  mojom::TransactionType tx_type;
  std::vector<std::string> tx_params;
  std::vector<std::string> tx_args;
  std::vector<uint8_t> data;

  // TXN: WETH → STG
  // sellEthForTokenToUniswapV3(bytes encodedPath,
  //                            uint256 minBuyAmount,
  //                            address recipient)
  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0x3598d8ab"  // function selector
      /*********************** HEAD (32x3 bytes) **********************/
      "0000000000000000000000000000000000000000000000000000000000000060"
      "0000000000000000000000000000000000000000000000030c1a39b13e25f498"
      "0000000000000000000000000000000000000000000000000000000000000000"

      /***************************** TAIL *****************************/
      // calldata reference position for encodedPath
      "000000000000000000000000000000000000000000000000000000000000002b"
      "c02aaa39b223fe8d0a0e5c4f27ead9083c756cc2"    // WETH
      "002710"                                      // POOL FEE
      "af5191b0de278c7286d6c7cc6ab6bb8a73ba2cd6"    // STG
      "000000000000000000000000000000000000000000"  // recipient address

      // extraneous tail segment to be ignored
      "869584cd0000000000000000000000003ce37278de6388532c3949ce4e886f36"
      "5b14fb560000000000000000000000000000000000000000000000f7834ab14c"
      "623f4f93",
      &data));
  auto tx_info = GetTransactionInfoFromData(data);
  ASSERT_NE(tx_info, std::nullopt);
  std::tie(tx_type, tx_params, tx_args) = *tx_info;

  EXPECT_EQ(tx_type, mojom::TransactionType::ETHSwap);

  ASSERT_EQ(tx_params.size(), 3UL);
  EXPECT_EQ(tx_params[0], "bytes");
  EXPECT_EQ(tx_params[1], "uint256");
  EXPECT_EQ(tx_params[2], "uint256");

  ASSERT_EQ(tx_args.size(), 3UL);
  EXPECT_EQ(tx_args[0],
            "0xc02aaa39b223fe8d0a0e5c4f27ead9083c756cc2"
            "af5191b0de278c7286d6c7cc6ab6bb8a73ba2cd6");
  EXPECT_EQ(tx_args[1], "");
  EXPECT_EQ(tx_args[2], "0x30c1a39b13e25f498");
}

TEST(EthDataParser, GetTransactionInfoFromDataSellTokenForEthToUniswapV3) {
  mojom::TransactionType tx_type;
  std::vector<std::string> tx_params;
  std::vector<std::string> tx_args;
  std::vector<uint8_t> data;

  // TXN: RSS3 → USDC → WETH
  // sellTokenForEthToUniswapV3(bytes encodedPath,
  //                            uint256 sellAmount,
  //                            uint256 minBuyAmount,
  //                            address recipient)
  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0x803ba26d"  // function selector
      /*********************** HEAD (32x4 bytes) **********************/
      "0000000000000000000000000000000000000000000000000000000000000080"
      "0000000000000000000000000000000000000000000000821ab0d44149800000"
      "0000000000000000000000000000000000000000000000000248b3366b6ffd46"
      "0000000000000000000000000000000000000000000000000000000000000000"

      /***************************** TAIL *****************************/
      // calldata reference position for encodedPath
      "0000000000000000000000000000000000000000000000000000000000000042"
      "c98d64da73a6616c42117b582e832812e7b8d57f"  // RSS3
      "000bb8"                                    // POOL FEE
      "a0b86991c6218b36c1d19d4a2e9eb0ce3606eb48"  // USDC
      "0001f4"                                    // POOL FEE
      "c02aaa39b223fe8d0a0e5c4f27ead9083c756cc2"  // WETH

      // extraneous tail segment to be ignored
      "0000000000000000000000000000000000000000000000000000000000008695"
      "84cd00000000000000000000000086003b044f70dac0abc80ac8957305b63708"
      "93ed0000000000000000000000000000000000000000000000c42194bea56247"
      "eafe",
      &data));
  auto tx_info = GetTransactionInfoFromData(data);
  ASSERT_NE(tx_info, std::nullopt);
  std::tie(tx_type, tx_params, tx_args) = *tx_info;

  EXPECT_EQ(tx_type, mojom::TransactionType::ETHSwap);

  ASSERT_EQ(tx_params.size(), 3UL);
  EXPECT_EQ(tx_params[0], "bytes");
  EXPECT_EQ(tx_params[1], "uint256");
  EXPECT_EQ(tx_params[2], "uint256");

  ASSERT_EQ(tx_args.size(), 3UL);
  EXPECT_EQ(tx_args[0],
            "0xc98d64da73a6616c42117b582e832812e7b8d57f"  // RSS3
            "a0b86991c6218b36c1d19d4a2e9eb0ce3606eb48"    // USDC
            "c02aaa39b223fe8d0a0e5c4f27ead9083c756cc2");  // WETH
  EXPECT_EQ(tx_args[1], "0x821ab0d44149800000");
  EXPECT_EQ(tx_args[2], "0x248b3366b6ffd46");
}

TEST(EthDataParser, GetTransactionInfoFromDataSellTokenForTokenToUniswapV3) {
  mojom::TransactionType tx_type;
  std::vector<std::string> tx_params;
  std::vector<std::string> tx_args;
  std::vector<uint8_t> data;

  // TXN: COW → WETH → USDC
  // sellTokenForTokenToUniswapV3(bytes encodedPath,
  //                              uint256 sellAmount,
  //                              uint256 minBuyAmount,
  //                              address recipient)
  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0x6af479b2"  // function selector

      /*********************** HEAD (32x4 bytes) **********************/
      "0000000000000000000000000000000000000000000000000000000000000080"
      "00000000000000000000000000000000000000000000004d12b6295c69ddebd5"
      "000000000000000000000000000000000000000000000000000000003b9aca00"
      "0000000000000000000000000000000000000000000000000000000000000000"

      /***************************** TAIL *****************************/
      // calldata reference position for encodedPath
      "0000000000000000000000000000000000000000000000000000000000000042"
      "def1ca1fb7fbcdc777520aa7f396b4e015f497ab"  // COW
      "002710"                                    // POOL FEE
      "c02aaa39b223fe8d0a0e5c4f27ead9083c756cc2"  // WETH
      "0001f4"                                    // POOL FEE
      "a0b86991c6218b36c1d19d4a2e9eb0ce3606eb48"  // USDC

      // extraneous tail segment to be ignored
      "000000000000000000000000000000000000000000000000000000000000869584cd0000"
      "0000000000000000000086003b044f70dac0abc80ac8957305b6370893ed000000000000"
      "0000000000000000000000000000000000495d35e8bf6247f2f1",
      &data));
  auto tx_info = GetTransactionInfoFromData(data);
  ASSERT_NE(tx_info, std::nullopt);
  std::tie(tx_type, tx_params, tx_args) = *tx_info;

  EXPECT_EQ(tx_type, mojom::TransactionType::ETHSwap);

  ASSERT_EQ(tx_params.size(), 3UL);
  EXPECT_EQ(tx_params[0], "bytes");
  EXPECT_EQ(tx_params[1], "uint256");
  EXPECT_EQ(tx_params[2], "uint256");

  ASSERT_EQ(tx_args.size(), 3UL);
  EXPECT_EQ(tx_args[0],
            "0xdef1ca1fb7fbcdc777520aa7f396b4e015f497ab"  // COW
            "c02aaa39b223fe8d0a0e5c4f27ead9083c756cc2"    // WETH
            "a0b86991c6218b36c1d19d4a2e9eb0ce3606eb48");  // USDC
  EXPECT_EQ(tx_args[1], "0x4d12b6295c69ddebd5");
  EXPECT_EQ(tx_args[2], "0x3b9aca00");
}

TEST(EthDataParser, GetTransactionInfoFromDataSellToUniswap) {
  mojom::TransactionType tx_type;
  std::vector<std::string> tx_params;
  std::vector<std::string> tx_args;
  std::vector<uint8_t> data;

  // TXN: USDC → WETH → LDO
  // sellToUniswap(address[] tokens,
  //               uint256 sellAmount,
  //               uint256 minBuyAmount,
  //               bool isSushi)
  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0xd9627aa4"  // function selector

      /*********************** HEAD (32x4 bytes) **********************/
      // calldata pointer to tokens
      "0000000000000000000000000000000000000000000000000000000000000080"
      "0000000000000000000000000000000000000000000000000000000077359400"
      "000000000000000000000000000000000000000000000016b28ec6ba93b8bb17"
      "0000000000000000000000000000000000000000000000000000000000000001"

      /***************************** TAIL *****************************/
      // calldata reference position for tokens
      "0000000000000000000000000000000000000000000000000000000000000003"
      "000000000000000000000000a0b86991c6218b36c1d19d4a2e9eb0ce3606eb48"
      "000000000000000000000000c02aaa39b223fe8d0a0e5c4f27ead9083c756cc2"
      "0000000000000000000000005a98fcbea516cf06857215779fd812ca3bef1b32"

      // extraneous tail segment to be ignored
      "869584cd00000000000000000000000086003b044f70dac0abc80ac8957305b63"
      "70893ed0000000000000000000000000000000000000000000000da92815dbd62"
      "4a716a",
      &data));
  auto tx_info = GetTransactionInfoFromData(data);
  ASSERT_NE(tx_info, std::nullopt);
  std::tie(tx_type, tx_params, tx_args) = *tx_info;

  EXPECT_EQ(tx_type, mojom::TransactionType::ETHSwap);

  ASSERT_EQ(tx_params.size(), 3UL);
  EXPECT_EQ(tx_params[0], "bytes");
  EXPECT_EQ(tx_params[1], "uint256");
  EXPECT_EQ(tx_params[2], "uint256");

  ASSERT_EQ(tx_args.size(), 3UL);
  EXPECT_EQ(tx_args[0],
            "0xa0b86991c6218b36c1d19d4a2e9eb0ce3606eb48"  // USDC
            "c02aaa39b223fe8d0a0e5c4f27ead9083c756cc2"    // WETH
            "5a98fcbea516cf06857215779fd812ca3bef1b32");  // LDO
  EXPECT_EQ(tx_args[1], "0x77359400");
  EXPECT_EQ(tx_args[2], "0x16b28ec6ba93b8bb17");
}

TEST(EthDataParser, GetTransactionInfoFromDataTransformERC20) {
  mojom::TransactionType tx_type;
  std::vector<std::string> tx_params;
  std::vector<std::string> tx_args;
  std::vector<uint8_t> data;

  // TXN: ETH → DAI
  // transformERC20(address inputToken,
  //                address outputToken,
  //                uint256 inputTokenAmount,
  //                uint256 minOutputTokenAmount,
  //                (uint32,bytes)[] transformations)
  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0x415565b0"  // function selector
      /*********************** HEAD (32x5 bytes) **********************/
      "000000000000000000000000eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"
      "0000000000000000000000008f3cf7ad23cd3cadbd9735aff958023239c6a063"
      "000000000000000000000000000000000000000000000000000000000902a721"
      "0000000000000000000000000000000000000000000000000000000005f5e100"
      "00000000000000000000000000000000000000000000000000000000000000a0"

      /***************************** TAIL *****************************/
      // size(transformations) = 3
      "0000000000000000000000000000000000000000000000000000000000000003"
      // transformations[0] offset = 3
      "0000000000000000000000000000000000000000000000000000000000000060"
      // transformations[1] offset = 8
      "0000000000000000000000000000000000000000000000000000000000000100"
      // transformations[2] offset = 34
      "0000000000000000000000000000000000000000000000000000000000000440"

      /*************** transformations[0] offset start ****************/
      // uint32
      "0000000000000000000000000000000000000000000000000000000000000004"
      // offset of bytes element
      "0000000000000000000000000000000000000000000000000000000000000040"
      // bytes element length
      "0000000000000000000000000000000000000000000000000000000000000040"
      // bytes element start
      "000000000000000000000000eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"
      "000000000000000000000000000000000000000000000000000000000902a721"
      // bytes element end

      /*************** transformations[1] offset start ****************/
      // uint32
      "0000000000000000000000000000000000000000000000000000000000000014"
      // offset of bytes element
      "0000000000000000000000000000000000000000000000000000000000000040"
      // bytes element length
      "00000000000000000000000000000000000000000000000000000000000002e0"
      // bytes element start
      "0000000000000000000000000000000000000000000000000000000000000020"
      "0000000000000000000000000000000000000000000000000000000000000001"
      "0000000000000000000000000d500b1d8e8ef31e21c99d1db9a6444d3adf1270"
      "0000000000000000000000008f3cf7ad23cd3cadbd9735aff958023239c6a063"
      "0000000000000000000000000000000000000000000000000000000000000140"
      "00000000000000000000000000000000000000000000000000000000000002a0"
      "00000000000000000000000000000000000000000000000000000000000002a0"
      "0000000000000000000000000000000000000000000000000000000000000260"
      "0000000000000000000000000000000000000000000000000000000005f5e100"
      "0000000000000000000000000000000000000000000000000000000000000000"
      "00000000000000000000000000000000000000000000000000000000000002a0"
      "0000000000000000000000000000000000000000000000000000000000000001"
      "0000000000000000000000000000000000000000000000000000000000000020"
      "0000000000000000000000000000000b446f646f563200000000000000000000"
      "000000000000000000000000000000000000000000000000000000000902a721"
      "0000000000000000000000000000000000000000000000000000000005f5e100"
      "0000000000000000000000000000000000000000000000000000000000000080"
      "0000000000000000000000000000000000000000000000000000000000000040"
      "000000000000000000000000d8547bf14887bc04638ae1163cb688770b279eac"
      "0000000000000000000000000000000000000000000000000000000000000001"
      "0000000000000000000000000000000000000000000000000000000000000001"
      "0000000000000000000000000000000000000000000000000000000000000000"
      "0000000000000000000000000000000000000000000000000000000000000000"
      // bytes element end

      /*************** transformations[2] offset start ****************/
      "000000000000000000000000000000000000000000000000000000000000000c"
      // uint32
      "0000000000000000000000000000000000000000000000000000000000000040"
      // offset of bytes element
      "00000000000000000000000000000000000000000000000000000000000000e0"
      // bytes element length
      "0000000000000000000000000000000000000000000000000000000000000020"
      // bytes element start
      "0000000000000000000000000000000000000000000000000000000000000040"
      "00000000000000000000000000000000000000000000000000000000000000a0"
      "0000000000000000000000000000000000000000000000000000000000000002"
      "0000000000000000000000000d500b1d8e8ef31e21c99d1db9a6444d3adf1270"
      "000000000000000000000000eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"
      "0000000000000000000000000000000000000000000000000000000000000000"
      "869584cd000000000000000000000000bd9420a98a7bd6b89765e5715e169481"
      "602d9c3d00000000000000000000000000000000963a25711797d1ed8fce2356"
      "b6f86b8a",  // bytes element end
      &data));
  auto tx_info = GetTransactionInfoFromData(data);
  ASSERT_NE(tx_info, std::nullopt);
  std::tie(tx_type, tx_params, tx_args) = *tx_info;

  EXPECT_EQ(tx_type, mojom::TransactionType::ETHSwap);

  ASSERT_EQ(tx_params.size(), 3UL);
  EXPECT_EQ(tx_params[0], "bytes");
  EXPECT_EQ(tx_params[1], "uint256");
  EXPECT_EQ(tx_params[2], "uint256");

  ASSERT_EQ(tx_args.size(), 3UL);
  EXPECT_EQ(tx_args[0],
            "0xeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"
            "8f3cf7ad23cd3cadbd9735aff958023239c6a063");
  EXPECT_EQ(tx_args[1], "0x902a721");
  EXPECT_EQ(tx_args[2], "0x5f5e100");
}

TEST(EthDataParser, GetTransactionInfoFromDataFillOtcOrderForETH) {
  mojom::TransactionType tx_type;
  std::vector<std::string> tx_params;
  std::vector<std::string> tx_args;
  std::vector<uint8_t> data;

  // TXN: USDC → ETH
  // fillOtcOrderForEth((address buyToken,
  //                     address sellToken,
  //                     uint128 buyAmount,
  //                     uint128 sellAmount,
  //                     address maker,
  //                     address taker,
  //                     address txOrigin,
  //                     uint256 expiryAndNonce),
  //                    (uint8 signatureType,
  //                     uint8 v,
  //                     bytes32 r,
  //                     bytes32 s),
  //                    uint128 takerTokenFillAmount)

  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0xa578efaf"  // function selector

      /***************************** HEAD ****************************/
      /************************ TUPLE INDEX 0 ************************/
      // buyToken
      "000000000000000000000000c02aaa39b223fe8d0a0e5c4f27ead9083c756cc2"
      // sellToken
      "000000000000000000000000a0b86991c6218b36c1d19d4a2e9eb0ce3606eb48"
      // buyAmount
      "000000000000000000000000000000000000000000000000003c11d06581812a"
      // sellAmount
      "0000000000000000000000000000000000000000000000000000000001c9c380"
      // maker
      "000000000000000000000000af0b0000f0210d0f421f0009c72406703b50506b"
      // taker
      "0000000000000000000000000000000000000000000000000000000000000000"
      // txOrigin
      "0000000000000000000000000a975d7b53f8da11e64196d53fb35532fea37e85"
      // expiryAndNonce
      "00000000641dc0e60000000000000000000000000000000000000000641dc08d"

      /************************ TUPLE INDEX 1 ************************/
      // signatureType
      "0000000000000000000000000000000000000000000000000000000000000003"
      // v
      "000000000000000000000000000000000000000000000000000000000000001b"
      // r
      "7ad29a4358f2b090fe87676b69a941b9304b751b7dd20ceb4aede5801342875d"
      // s
      "37c1445a8ea241a1ddeb91628a685fdbaf1b31701a1b4782ee9f239b27de8da7"

      /************************ TUPLE INDEX 2 ************************/
      // takerTokenFillAmount
      "0000000000000000000000000000000000000000000000000000000001c9c380"

      // Extraneous HEAD data to be ignored
      "869584cd00000000000000000000000010000000000000000000000000000000"
      "000000110000000000000000000000000000000000000000000000b68c522ab9"
      "641dc08d",
      &data));

  auto tx_info = GetTransactionInfoFromData(data);
  ASSERT_NE(tx_info, std::nullopt);
  std::tie(tx_type, tx_params, tx_args) = *tx_info;

  EXPECT_EQ(tx_type, mojom::TransactionType::ETHSwap);

  ASSERT_EQ(tx_params.size(), 3UL);
  EXPECT_EQ(tx_params[0], "bytes");
  EXPECT_EQ(tx_params[1], "uint128");
  EXPECT_EQ(tx_params[2], "uint128");

  ASSERT_EQ(tx_args.size(), 3UL);
  EXPECT_EQ(tx_args[0],
            "0xa0b86991c6218b36c1d19d4a2e9eb0ce3606eb48"  // USDC
            "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee");  // ETH
  EXPECT_EQ(tx_args[1], "0x1c9c380");
  EXPECT_EQ(tx_args[2], "0x3c11d06581812a");
}

TEST(EthDataParser, GetTransactionInfoFromDataFillOtcOrderWithETH) {
  mojom::TransactionType tx_type;
  std::vector<std::string> tx_params;
  std::vector<std::string> tx_args;
  std::vector<uint8_t> data;

  // TXN: ETH → USDC
  // fillOtcOrderWithEth((address buyToken,
  //                      address sellToken,
  //                      uint128 buyAmount,
  //                      uint128 sellAmount,
  //                      address maker,
  //                      address taker,
  //                      address txOrigin,
  //                      uint256 expiryAndNonce),
  //                     (uint8 signatureType,
  //                      uint8 v,
  //                      bytes32 r,
  //                      bytes32 s))

  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0x706394d5"  // function selector

      /***************************** HEAD ****************************/
      /************************ TUPLE INDEX 0 ************************/
      // buyToken
      "000000000000000000000000a0b86991c6218b36c1d19d4a2e9eb0ce3606eb48"
      // sellToken
      "000000000000000000000000c02aaa39b223fe8d0a0e5c4f27ead9083c756cc2"
      // buyAmount
      "0000000000000000000000000000000000000000000000000000000001c9c380"
      // sellAmount
      "000000000000000000000000000000000000000000000000003d407736bd1262"
      // maker
      "000000000000000000000000af0b0000f0210d0f421f0009c72406703b50506b"
      // taker
      "0000000000000000000000000000000000000000000000000000000000000000"
      // txOrigin
      "0000000000000000000000000a975d7b53f8da11e64196d53fb35532fea37e85"
      // expiryAndNonce
      "00000000641df0fc0000000000000000000000000000000000000000641df0a3"

      /************************ TUPLE INDEX 1 ************************/
      // signatureType
      "0000000000000000000000000000000000000000000000000000000000000003"
      // v
      "000000000000000000000000000000000000000000000000000000000000001b"
      // r
      "698ec17fa0d923fc71072f04cc605ce1e0701eb684e3ec86da60fc4056a8d1cf"
      // s
      "79c95c461f9e1899f85677b2d5873d128d49007c98d2db482ad0c074f3da91cf"

      // Extraneous HEAD data to be ignored
      "869584cd00000000000000000000000010000000000000000000000000000000"
      "00000011000000000000000000000000000000000000000000000056b6e7d5c8"
      "641df0a3",
      &data));

  auto tx_info = GetTransactionInfoFromData(data);
  ASSERT_NE(tx_info, std::nullopt);
  std::tie(tx_type, tx_params, tx_args) = *tx_info;

  EXPECT_EQ(tx_type, mojom::TransactionType::ETHSwap);

  ASSERT_EQ(tx_params.size(), 3UL);
  EXPECT_EQ(tx_params[0], "bytes");
  EXPECT_EQ(tx_params[1], "uint128");
  EXPECT_EQ(tx_params[2], "uint128");

  ASSERT_EQ(tx_args.size(), 3UL);
  EXPECT_EQ(tx_args[0],
            "0xeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"  // ETH
            "a0b86991c6218b36c1d19d4a2e9eb0ce3606eb48");  // USDC
  EXPECT_EQ(tx_args[1], "0x3d407736bd1262");
  EXPECT_EQ(tx_args[2], "0x1c9c380");
}

TEST(EthDataParser, GetTransactionInfoFromDataFillOtcOrder) {
  mojom::TransactionType tx_type;
  std::vector<std::string> tx_params;
  std::vector<std::string> tx_args;
  std::vector<uint8_t> data;

  // TXN: USDC → USDT
  // fillOtcOrder((address buyToken,
  //               address sellToken,
  //               uint128 buyAmount,
  //               uint128 sellAmount,
  //               address maker,
  //               address taker,
  //               address txOrigin,
  //               uint256 expiryAndNonce),
  //              (uint8 signatureType,
  //               uint8 v,
  //               bytes32 r,
  //               bytes32 s),
  //              uint128 takerTokenFillAmount)

  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0xdac748d4"  // function selector

      /***************************** HEAD ****************************/
      /************************ TUPLE INDEX 0 ************************/
      // buyToken
      "000000000000000000000000dac17f958d2ee523a2206206994597c13d831ec7"
      // sellToken
      "000000000000000000000000a0b86991c6218b36c1d19d4a2e9eb0ce3606eb48"
      // buyAmount
      "0000000000000000000000000000000000000000000000000000000001c6bad5"
      // sellAmount
      "0000000000000000000000000000000000000000000000000000000001c9c380"
      // maker
      "000000000000000000000000af0b0000f0210d0f421f0009c72406703b50506b"
      // taker
      "0000000000000000000000000000000000000000000000000000000000000000"
      // txOrigin
      "0000000000000000000000000a975d7b53f8da11e64196d53fb35532fea37e85"
      // expiryAndNonce
      "00000000641e09580000000000000000000000000000000000000000641e08ff"

      /************************ TUPLE INDEX 1 ************************/
      // signatureType
      "0000000000000000000000000000000000000000000000000000000000000003"
      // v
      "000000000000000000000000000000000000000000000000000000000000001c"
      // r
      "de2afeb6c575ec3fbce0a2f52eeee77ed2d08df1bd3d0888f9fa65cc5184e98a"
      // s
      "6a6dbfa0c3444521b4bdd4d2293e6cc5013d21d6758e38e4e9f2e0f106aadeab"

      /************************ TUPLE INDEX 2 ************************/
      // takerTokenFillAmount
      "0000000000000000000000000000000000000000000000000000000001c9c380"

      // Extraneous HEAD data to be ignored
      "869584cd00000000000000000000000010000000000000000000000000000000"
      "000000110000000000000000000000000000000000000000000000423216738d"
      "641e08ff",
      &data));

  auto tx_info = GetTransactionInfoFromData(data);
  ASSERT_NE(tx_info, std::nullopt);
  std::tie(tx_type, tx_params, tx_args) = *tx_info;

  EXPECT_EQ(tx_type, mojom::TransactionType::ETHSwap);

  ASSERT_EQ(tx_params.size(), 3UL);
  EXPECT_EQ(tx_params[0], "bytes");
  EXPECT_EQ(tx_params[1], "uint128");
  EXPECT_EQ(tx_params[2], "uint128");

  ASSERT_EQ(tx_args.size(), 3UL);
  EXPECT_EQ(tx_args[0],
            "0xa0b86991c6218b36c1d19d4a2e9eb0ce3606eb48"  // USDC
            "dac17f958d2ee523a2206206994597c13d831ec7");  // USDT
  EXPECT_EQ(tx_args[1], "0x1c9c380");
  EXPECT_EQ(tx_args[2], "0x1c6bad5");
}

TEST(EthDataParser, GetTransactionInfoFromDataCowOrderSellEth) {
  mojom::TransactionType tx_type;
  std::vector<std::string> tx_params;
  std::vector<std::string> tx_args;
  std::vector<uint8_t> data;

  // TXN: XDAI → USDC
  // Function:
  // createOrder((address buyToken,
  //              address receiver,
  //              uint256 sellAmount,
  //              uint256 buyAmount,
  //              bytes32 appData,
  //              uint256 feeAmount,
  //              uint32 validTo,
  //              bool partiallyFillable,
  //              int64 quoteId))

  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0x322bba21"  // function selector

      /***************************** HEAD ****************************/
      /************************ TUPLE INDEX 0 ************************/
      // buyToken
      "000000000000000000000000ddafbb505ad214d7b80b1f830fccc89b60fb7a83"
      // receiver
      "000000000000000000000000a92d461a9a988a7f11ec285d39783a637fdd6ba4"
      // sellAmount
      "000000000000000000000000000000000000000000000000004967cb9ebd8176"
      // buyAmount
      "0000000000000000000000000000000000000000000000000000000000004f1e"
      // appData
      "c21ba2efa76e703f0a9a496e09ea7d0e66d907a47ba8f109a3a760720504ab32"
      // feeAmount
      "000000000000000000000000000000000000000000000000000107c0fe0dc060"
      // validTo
      "00000000000000000000000000000000000000000000000000000000650b4580"
      // partiallyFillable
      "0000000000000000000000000000000000000000000000000000000000000000"
      // quoteId
      "000000000000000000000000000000000000000000000000000000000332b123",
      &data));

  auto tx_info = GetTransactionInfoFromData(data);
  ASSERT_NE(tx_info, std::nullopt);
  std::tie(tx_type, tx_params, tx_args) = *tx_info;

  EXPECT_EQ(tx_type, mojom::TransactionType::ETHSwap);

  ASSERT_EQ(tx_params.size(), 3UL);
  EXPECT_EQ(tx_params[0], "bytes");
  EXPECT_EQ(tx_params[1], "uint256");
  EXPECT_EQ(tx_params[2], "uint256");

  ASSERT_EQ(tx_args.size(), 3UL);
  EXPECT_EQ(tx_args[0],
            "0xeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"  // XDAI
            "ddafbb505ad214d7b80b1f830fccc89b60fb7a83");  // USDC
  EXPECT_EQ(tx_args[1], "0x4967cb9ebd8176");  // 0.02066179753911948 XDAI
  EXPECT_EQ(tx_args[2], "0x4f1e");            // 0.020254 USDC
}

TEST(EthDataParser, GetTransactionInfoFromFilForward) {
  mojom::TransactionType tx_type;
  std::vector<std::string> tx_params;
  std::vector<std::string> tx_args;

  std::vector<uint8_t> data;
  ASSERT_TRUE(
      PrefixedHexStringToBytes("0xd948d468"  // forward(bytes)
                               "00000000000000000000000000000000000000000000000"
                               "00000000000000020"  // bytes offset
                               "00000000000000000000000000000000000000000000000"
                               "00000000000000015"  // bytes length
                               "01d15cf6d7364d8b4dab9d90dc5699d1a78cf729c100000"
                               "00000000000000000",  // bytes content
                               &data));
  auto tx_info = GetTransactionInfoFromData(data);
  ASSERT_NE(tx_info, std::nullopt);
  std::tie(tx_type, tx_params, tx_args) = *tx_info;

  EXPECT_EQ(tx_type, mojom::TransactionType::ETHFilForwarderTransfer);
  ASSERT_EQ(tx_params.size(), 1UL);
  ASSERT_EQ(tx_args.size(), 1UL);
  ASSERT_EQ(tx_params[0], "bytes");
  ASSERT_EQ(tx_args[0], "0x01d15cf6d7364d8b4dab9d90dc5699d1a78cf729c1");
}

TEST(EthDataParser, GetTransactionInfoFromDataLiFiSwapTokensGeneric) {
  mojom::TransactionType tx_type;
  std::vector<std::string> tx_params;
  std::vector<std::string> tx_args;
  std::vector<uint8_t> data;

  // TXN: token → token
  // Function:
  // swapTokensGeneric(bytes32 transactionId,
  //                   string integrator,
  //                   string referrer,
  //                   address receiver,
  //                   uint256 minAmountOut,
  //                   (address callTo,
  //                    address approveTo,
  //                    address sendingAssetId,
  //                    address receivingAssetId,
  //                    uint256 fromAmount,
  //                    bytes callData,
  //                    bool requiresDeposit)[] swapData)

  // Swap 0.504913 USDC.e → 0.6797397017301765 MATIC
  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0x4630a0d8"  // function selector

      /***************************** HEAD ****************************/
      // transactionId
      "a45ad3e99c879cbd3103bcb2a36fc940d9c017500e587fc63b796ff8f4d28d89"
      // integrator (offset)
      "00000000000000000000000000000000000000000000000000000000000000c0"
      // referrer
      "0000000000000000000000000000000000000000000000000000000000000100"
      // receiver
      "000000000000000000000000a92d461a9a988a7f11ec285d39783a637fdd6ba4"
      // minAmountOut
      "000000000000000000000000000000000000000000000000096eeba8455b6e35"

      // offset to start data part of swapData
      "0000000000000000000000000000000000000000000000000000000000000160"

      // offset to start data part of integrator
      // size of integrator string
      "0000000000000000000000000000000000000000000000000000000000000005"
      // integrator string
      "6272617665000000000000000000000000000000000000000000000000000000"

      // offset to start data part of referrer
      // size of referrer string
      "000000000000000000000000000000000000000000000000000000000000002a"
      // referrer string
      "3078303030303030303030303030303030303030303030303030303030303030"
      "3030303030303030303000000000000000000000000000000000000000000000"

      // size(swapData) = 2
      "0000000000000000000000000000000000000000000000000000000000000002"
      // swapData[0] offset
      "0000000000000000000000000000000000000000000000000000000000000040"
      // swapData[1] offset
      "00000000000000000000000000000000000000000000000000000000000001e0"

      /************************** swapData[0] *************************/
      // callTo
      "000000000000000000000000bd6c7b0d2f68c2b7805d88388319cfb6ecb50ea9"
      // approveTo
      "000000000000000000000000bd6c7b0d2f68c2b7805d88388319cfb6ecb50ea9"
      // sendingAssetId
      "0000000000000000000000002791bca1f2de4661ed88a30c99a7a9449aa84174"
      // receivingAssetId
      "0000000000000000000000002791bca1f2de4661ed88a30c99a7a9449aa84174"
      // fromAmount
      "000000000000000000000000000000000000000000000000000000000007b451"
      // callData
      "00000000000000000000000000000000000000000000000000000000000000e0"
      // requiresDeposit
      "0000000000000000000000000000000000000000000000000000000000000001"

      "0000000000000000000000000000000000000000000000000000000000000084"
      "eedd56e10000000000000000000000002791bca1f2de4661ed88a30c99a7a944"
      "9aa8417400000000000000000000000000000000000000000000000000000000"
      "00000dcd00000000000000000000000000000000000000000000000000000000"
      "00000373000000000000000000000000bd9420a98a7bd6b89765e5715e169481"
      "602d9c3d00000000000000000000000000000000000000000000000000000000"

      /************************** swapData[1] *************************/
      // callTo
      "000000000000000000000000c0788a3ad43d79aa53b09c2eacc313a787d1d607"
      // approveTo
      "000000000000000000000000c0788a3ad43d79aa53b09c2eacc313a787d1d607"
      // sendingAssetId
      "0000000000000000000000002791bca1f2de4661ed88a30c99a7a9449aa84174"
      // receivingAssetId
      "0000000000000000000000000000000000000000000000000000000000000000"
      // fromAmount
      "000000000000000000000000000000000000000000000000000000000007a310"
      // callData
      "00000000000000000000000000000000000000000000000000000000000000e0"
      // requiresDeposit
      "0000000000000000000000000000000000000000000000000000000000000000"

      "0000000000000000000000000000000000000000000000000000000000000104"
      "18cbafe500000000000000000000000000000000000000000000000000000000"
      "0007a310000000000000000000000000000000000000000000000000096eeba8"
      "455b6e3500000000000000000000000000000000000000000000000000000000"
      "000000a00000000000000000000000001231deb6f5749ef6ce6943a275a1d3e7"
      "486f4eae00000000000000000000000000000000000000000000000000000000"
      "66354c0400000000000000000000000000000000000000000000000000000000"
      "000000020000000000000000000000002791bca1f2de4661ed88a30c99a7a944"
      "9aa841740000000000000000000000000d500b1d8e8ef31e21c99d1db9a6444d"
      "3adf127000000000000000000000000000000000000000000000000000000000",
      &data));

  auto tx_info = GetTransactionInfoFromData(data);
  ASSERT_NE(tx_info, std::nullopt);

  std::tie(tx_type, tx_params, tx_args) = *tx_info;

  EXPECT_EQ(tx_type, mojom::TransactionType::ETHSwap);

  ASSERT_EQ(tx_params.size(), 3UL);
  EXPECT_EQ(tx_params[0], "bytes");
  EXPECT_EQ(tx_params[1], "uint256");
  EXPECT_EQ(tx_params[2], "uint256");

  ASSERT_EQ(tx_args.size(), 3UL);
  EXPECT_EQ(tx_args[0],
            "0x2791bca1f2de4661ed88a30c99a7a9449aa84174"  // USDC.e
            "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee");  // MATIC
  EXPECT_EQ(tx_args[1], "0x7b451");                       // 0.504913 USDC.e
  EXPECT_EQ(tx_args[2], "0x96eeba8455b6e35");  // 0.6797397017301765 MATIC

  // Swap 1 MATIC → Y USDC.e
  ASSERT_TRUE(PrefixedHexStringToBytes(
      "0x4630a0d8"  // function selector

      /***************************** HEAD ****************************/
      // transactionId
      "73bc2c896381e1296eefd4ddbbe7efbb62ae1d1968de6d764364d762f1fd9b9e"
      // integrator (offset)
      "00000000000000000000000000000000000000000000000000000000000000c0"
      // referrer
      "0000000000000000000000000000000000000000000000000000000000000100"
      // receiver
      "000000000000000000000000a92d461a9a988a7f11ec285d39783a637fdd6ba4"
      // minAmountOut
      "0000000000000000000000000000000000000000000000000000000000098647"

      // offset to start data part of swapData
      "0000000000000000000000000000000000000000000000000000000000000160"

      // offset to start data part of integrator
      // size of integrator string
      "0000000000000000000000000000000000000000000000000000000000000005"
      // integrator string
      "6272617665000000000000000000000000000000000000000000000000000000"

      // offset to start data part of referrer
      // size of referrer string
      "000000000000000000000000000000000000000000000000000000000000002a"
      "3078303030303030303030303030303030303030303030303030303030303030"
      "3030303030303030303000000000000000000000000000000000000000000000"

      // size(swapData) = 1
      "0000000000000000000000000000000000000000000000000000000000000001"
      // swapData[0] offset
      "0000000000000000000000000000000000000000000000000000000000000020"

      /************************** swapData[0] *************************/
      // callTo
      "000000000000000000000000a5e0829caced8ffdd4de3c43696c57f7d7a678ff"
      // approveTo
      "000000000000000000000000a5e0829caced8ffdd4de3c43696c57f7d7a678ff"
      // sendingAssetId
      "0000000000000000000000000000000000000000000000000000000000000000"
      // receivingAssetId
      "0000000000000000000000002791bca1f2de4661ed88a30c99a7a9449aa84174"
      // fromAmount
      "0000000000000000000000000000000000000000000000000de0b6b3a7640000"
      // callData
      "00000000000000000000000000000000000000000000000000000000000000e0"
      // requiresDeposit
      "0000000000000000000000000000000000000000000000000000000000000001"

      "00000000000000000000000000000000000000000000000000000000000000e4"
      "7ff36ab500000000000000000000000000000000000000000000000000000000"
      "0009864700000000000000000000000000000000000000000000000000000000"
      "000000800000000000000000000000001231deb6f5749ef6ce6943a275a1d3e7"
      "486f4eae00000000000000000000000000000000000000000000000000000000"
      "6668bc8800000000000000000000000000000000000000000000000000000000"
      "000000020000000000000000000000000d500b1d8e8ef31e21c99d1db9a6444d"
      "3adf12700000000000000000000000002791bca1f2de4661ed88a30c99a7a944"
      "9aa8417400000000000000000000000000000000000000000000000000000000",
      &data));

  tx_info = GetTransactionInfoFromData(data);
  ASSERT_NE(tx_info, std::nullopt);

  std::tie(tx_type, tx_params, tx_args) = *tx_info;

  EXPECT_EQ(tx_type, mojom::TransactionType::ETHSwap);

  ASSERT_EQ(tx_params.size(), 3UL);
  EXPECT_EQ(tx_params[0], "bytes");
  EXPECT_EQ(tx_params[1], "uint256");
  EXPECT_EQ(tx_params[2], "uint256");

  ASSERT_EQ(tx_args.size(), 3UL);
  EXPECT_EQ(tx_args[0],
            "0xeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"  // MATIC
            "2791bca1f2de4661ed88a30c99a7a9449aa84174");  // USDC.e
  EXPECT_EQ(tx_args[1], "0xde0b6b3a7640000");             // 1 MATIC
  EXPECT_EQ(tx_args[2], "0x98647");                       // 0.624199 USDC.e
}

}  // namespace brave_wallet
