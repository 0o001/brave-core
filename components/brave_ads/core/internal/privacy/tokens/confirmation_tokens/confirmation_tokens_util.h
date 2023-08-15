/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_ADS_CORE_INTERNAL_PRIVACY_TOKENS_CONFIRMATION_TOKENS_CONFIRMATION_TOKENS_UTIL_H_
#define BRAVE_COMPONENTS_BRAVE_ADS_CORE_INTERNAL_PRIVACY_TOKENS_CONFIRMATION_TOKENS_CONFIRMATION_TOKENS_UTIL_H_

#include "brave/components/brave_ads/core/internal/privacy/tokens/confirmation_tokens/confirmation_token_info.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace brave_ads::privacy {

absl::optional<ConfirmationTokenInfo> MaybeGetConfirmationToken();

void AddConfirmationTokens(const ConfirmationTokenList& confirmation_tokens);

bool RemoveConfirmationToken(const ConfirmationTokenInfo& confirmation_token);
void RemoveConfirmationTokens(const ConfirmationTokenList& confirmation_tokens);
void RemoveAllConfirmationTokens();

bool ConfirmationTokenExists(const ConfirmationTokenInfo& confirmation_token);

bool ConfirmationTokensIsEmpty();

int ConfirmationTokenCount();

[[nodiscard]] bool IsValid(const ConfirmationTokenInfo& confirmation_token);

}  // namespace brave_ads::privacy

#endif  // BRAVE_COMPONENTS_BRAVE_ADS_CORE_INTERNAL_PRIVACY_TOKENS_CONFIRMATION_TOKENS_CONFIRMATION_TOKENS_UTIL_H_
