/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_REWARDS_CORE_LEDGER_H_
#define BRAVE_COMPONENTS_BRAVE_REWARDS_CORE_LEDGER_H_

#include <memory>
#include <string>
#include <vector>

#include "base/containers/flat_map.h"
#include "base/functional/callback_forward.h"
#include "base/types/expected.h"
#include "brave/components/brave_rewards/common/mojom/bat_ledger.mojom.h"
#include "brave/components/brave_rewards/common/mojom/ledger_types.mojom.h"
#include "brave/components/brave_rewards/core/mojom_structs.h"

namespace ledger {

using AttestPromotionCallback = mojom::Ledger::AttestPromotionCallback;

using ClaimPromotionCallback = mojom::Ledger::ClaimPromotionCallback;

using ConnectExternalWalletCallback =
    mojom::Ledger::ConnectExternalWalletCallback;

using ConnectExternalWalletResult =
    base::expected<void, mojom::ConnectExternalWalletError>;

using CreateRewardsWalletCallback = mojom::Ledger::CreateRewardsWalletCallback;

using FetchBalanceCallback = mojom::Ledger::FetchBalanceCallback;

using FetchBalanceResult =
    base::expected<mojom::BalancePtr, mojom::FetchBalanceError>;

using FetchPromotionsCallback = mojom::Ledger::FetchPromotionsCallback;

using GetExternalWalletCallback = mojom::Ledger::GetExternalWalletCallback;

using GetExternalWalletResult =
    base::expected<mojom::ExternalWalletPtr, mojom::GetExternalWalletError>;

using GetRewardsParametersCallback =
    mojom::Ledger::GetRewardsParametersCallback;

using GetRewardsWalletCallback = mojom::Ledger::GetRewardsWalletCallback;

using LoadURLCallback = base::OnceCallback<void(mojom::UrlResponsePtr)>;

using PostSuggestionsClaimCallback =
    base::OnceCallback<void(mojom::Result, std::string)>;

using ResultCallback = base::OnceCallback<void(mojom::Result)>;

using RunDBTransactionCallback =
    base::OnceCallback<void(mojom::DBCommandResponsePtr)>;

// Legacy callbacks:

using ContributionInfoListCallback =
    std::function<void(std::vector<mojom::ContributionInfoPtr>)>;

using GetActivityInfoListCallback =
    std::function<void(std::vector<mojom::PublisherInfoPtr>)>;

using GetAllMonthlyReportIdsCallback =
    std::function<void(const std::vector<std::string>&)>;

using GetAllPromotionsCallback =
    std::function<void(base::flat_map<std::string, mojom::PromotionPtr>)>;

using GetBalanceReportCallback =
    std::function<void(mojom::Result, mojom::BalanceReportInfoPtr)>;

using GetBalanceReportListCallback = std::function<void(
    std::vector<mojom::BalanceReportInfoPtr>)>;  // TODO(sszaloki): unused?

using GetContributionReportCallback =
    std::function<void(std::vector<mojom::ContributionReportInfoPtr>)>;

using GetEventLogsCallback =
    std::function<void(std::vector<mojom::EventLogPtr>)>;

using GetExcludedListCallback =
    std::function<void(std::vector<mojom::PublisherInfoPtr>)>;

using GetMonthlyReportCallback =
    std::function<void(mojom::Result, mojom::MonthlyReportInfoPtr)>;

using GetOneTimeTipsCallback =
    std::function<void(std::vector<mojom::PublisherInfoPtr>)>;

using GetPendingContributionsCallback =
    std::function<void(std::vector<mojom::PendingContributionInfoPtr>)>;

using GetPendingContributionsTotalCallback = std::function<void(double)>;

using GetPublisherBannerCallback =
    std::function<void(mojom::PublisherBannerPtr)>;

using GetPublisherInfoCallback =
    std::function<void(mojom::Result, mojom::PublisherInfoPtr)>;

using GetPublisherPanelInfoCallback =
    std::function<void(mojom::Result, mojom::PublisherInfoPtr)>;

using GetRecurringTipsCallback =
    std::function<void(std::vector<mojom::PublisherInfoPtr>)>;

using GetTransactionReportCallback =
    std::function<void(std::vector<mojom::TransactionReportInfoPtr>)>;

using LegacyLoadURLCallback = std::function<void(mojom::UrlResponsePtr)>;

using LegacyResultCallback = std::function<void(mojom::Result)>;

using LegacyRunDBTransactionCallback =
    std::function<void(mojom::DBCommandResponsePtr)>;

using PublisherInfoCallback =
    std::function<void(mojom::Result, mojom::PublisherInfoPtr)>;

using RefreshPublisherCallback = std::function<void(mojom::PublisherStatus)>;

using SKUOrderCallback = std::function<void(mojom::Result, const std::string&)>;

using UnverifiedPublishersCallback =
    std::function<void(std::vector<std::string>&&)>;

}  // namespace ledger

namespace ledger {

inline mojom::Environment _environment = mojom::Environment::PRODUCTION;
inline bool is_debug = false;
inline bool is_testing = false;
inline int state_migration_target_version_for_testing = -1;
inline int reconcile_interval = 0;  // minutes
inline int retry_interval = 0;      // seconds

}  // namespace ledger

#endif  // BRAVE_COMPONENTS_BRAVE_REWARDS_CORE_LEDGER_H_
