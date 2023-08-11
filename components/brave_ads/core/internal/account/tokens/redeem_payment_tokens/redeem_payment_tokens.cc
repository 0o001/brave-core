/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_ads/core/internal/account/tokens/redeem_payment_tokens/redeem_payment_tokens.h"

#include <utility>

#include "base/functional/bind.h"
#include "base/time/time.h"
#include "base/values.h"
#include "brave/components/brave_ads/common/interfaces/brave_ads.mojom.h"
#include "brave/components/brave_ads/core/internal/account/tokens/redeem_payment_tokens/redeem_payment_tokens_util.h"
#include "brave/components/brave_ads/core/internal/account/tokens/redeem_payment_tokens/url_request_builders/redeem_payment_tokens_url_request_builder.h"
#include "brave/components/brave_ads/core/internal/account/tokens/redeem_payment_tokens/user_data/redeem_payment_tokens_user_data_builder.h"
#include "brave/components/brave_ads/core/internal/ads_client_helper.h"
#include "brave/components/brave_ads/core/internal/common/logging_util.h"
#include "brave/components/brave_ads/core/internal/common/time/time_formatting_util.h"
#include "brave/components/brave_ads/core/internal/common/url/url_request_string_util.h"
#include "brave/components/brave_ads/core/internal/common/url/url_response_string_util.h"
#include "brave/components/brave_ads/core/internal/privacy/tokens/payment_tokens/payment_token_util.h"
#include "net/http/http_status_code.h"

namespace brave_ads {

namespace {
constexpr base::TimeDelta kRetryAfter = base::Minutes(1);
}  // namespace

RedeemPaymentTokens::RedeemPaymentTokens() = default;

RedeemPaymentTokens::~RedeemPaymentTokens() {
  delegate_ = nullptr;
}

void RedeemPaymentTokens::MaybeRedeemAfterDelay(const WalletInfo& wallet) {
  CHECK(wallet.IsValid());

  if (is_processing_ || timer_.IsRunning() || retry_timer_.IsRunning()) {
    return;
  }

  wallet_ = wallet;

  const base::Time redeem_at = timer_.Start(
      FROM_HERE, CalculateDelayBeforeRedeemingTokens(),
      base::BindOnce(&RedeemPaymentTokens::Redeem, base::Unretained(this)));

  BLOG(1, "Redeem payment tokens " << FriendlyDateAndTime(redeem_at));
}

///////////////////////////////////////////////////////////////////////////////

void RedeemPaymentTokens::Redeem() {
  CHECK(!is_processing_);

  BLOG(1, "RedeemPaymentTokens");
  BLOG(2, "PUT /v3/confirmation/payment/{paymentId}");

  if (privacy::PaymentTokensIsEmpty()) {
    BLOG(1, "No payment tokens to redeem");
    return ScheduleNextRedemption();
  }

  is_processing_ = true;

  BuildRedeemPaymentTokensUserData(
      privacy::GetAllPaymentTokens(),
      base::BindOnce(
          &RedeemPaymentTokens::BuildRedeemPaymentTokensUserDataCallback,
          weak_factory_.GetWeakPtr()));
}

void RedeemPaymentTokens::BuildRedeemPaymentTokensUserDataCallback(
    base::Value::Dict user_data) {
  const privacy::PaymentTokenList& payment_tokens =
      privacy::GetAllPaymentTokens();

  RedeemPaymentTokensUrlRequestBuilder url_request_builder(
      wallet_, payment_tokens, std::move(user_data));
  mojom::UrlRequestInfoPtr url_request = url_request_builder.Build();
  BLOG(6, UrlRequestToString(url_request));
  BLOG(7, UrlRequestHeadersToString(url_request));

  AdsClientHelper::GetInstance()->UrlRequest(
      std::move(url_request),
      base::BindOnce(&RedeemPaymentTokens::RedeemCallback,
                     weak_factory_.GetWeakPtr(), payment_tokens));
}

void RedeemPaymentTokens::RedeemCallback(
    const privacy::PaymentTokenList& payment_tokens,
    const mojom::UrlResponseInfo& url_response) {
  BLOG(1, "OnRedeemPaymentTokens");

  BLOG(6, UrlResponseToString(url_response));
  BLOG(7, UrlResponseHeadersToString(url_response));

  if (url_response.status_code != net::HTTP_OK) {
    return FailedToRedeem(/*should_retry*/ true);
  }

  SuccessfullyRedeemed(payment_tokens);
}

void RedeemPaymentTokens::SuccessfullyRedeemed(
    const privacy::PaymentTokenList& payment_tokens) {
  BLOG(1, "Successfully redeemed payment tokens");

  is_processing_ = false;

  StopRetrying();

  privacy::RemovePaymentTokens(payment_tokens);

  if (delegate_) {
    delegate_->OnDidRedeemPaymentTokens(payment_tokens);
  }

  ScheduleNextRedemption();
}

void RedeemPaymentTokens::FailedToRedeem(const bool should_retry) {
  BLOG(1, "Failed to redeem payment tokens");

  if (delegate_) {
    delegate_->OnFailedToRedeemPaymentTokens();
  }

  if (should_retry) {
    Retry();
  }
}

void RedeemPaymentTokens::ScheduleNextRedemption() {
  const base::Time redeem_at = ScheduleNextTokenRedemptionAt();
  SetNextTokenRedemptionAt(redeem_at);

  if (delegate_) {
    delegate_->OnDidScheduleNextPaymentTokenRedemption(redeem_at);
  }

  MaybeRedeemAfterDelay(wallet_);
}

void RedeemPaymentTokens::Retry() {
  const base::Time retry_at = retry_timer_.StartWithPrivacy(
      FROM_HERE, kRetryAfter,
      base::BindOnce(&RedeemPaymentTokens::RetryCallback,
                     weak_factory_.GetWeakPtr()));

  BLOG(1, "Retry redeeming payment tokens " << FriendlyDateAndTime(retry_at));

  if (delegate_) {
    delegate_->OnWillRetryRedeemingPaymentTokens(retry_at);
  }
}

void RedeemPaymentTokens::RetryCallback() {
  BLOG(1, "Retry redeeming payment tokens");

  if (delegate_) {
    delegate_->OnDidRetryRedeemingPaymentTokens();
  }

  is_processing_ = false;

  Redeem();
}

void RedeemPaymentTokens::StopRetrying() {
  retry_timer_.Stop();
}

}  // namespace brave_ads
