/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/services/bat_rewards/rewards_engine_factory.h"

#include <utility>

namespace brave_rewards::internal {

RewardsEngineFactory::RewardsEngineFactory(
    mojo::PendingReceiver<mojom::RewardsEngineFactory> receiver)
    : receiver_(this, std::move(receiver)) {}

RewardsEngineFactory::~RewardsEngineFactory() = default;

void RewardsEngineFactory::CreateRewardsEngine(
    mojo::PendingAssociatedReceiver<mojom::RewardsEngine> engine_receiver,
    mojo::PendingAssociatedRemote<mojom::RewardsEngineClient> client_remote,
    mojom::RewardsEngineOptionsPtr options,
    CreateRewardsEngineCallback callback) {
  if (!engine_) {
    // Set global options for the current process. This must be done before
    // the `RewardsEngineImpl` constructor is invoked so that subobjects see the
    // correct values.
    _environment = options->environment;
    is_testing = options->is_testing;
    is_debug = options->is_debug;
    state_migration_target_version_for_testing =
        options->state_migration_target_version_for_testing;
    reconcile_interval = options->reconcile_interval;
    retry_interval = options->retry_interval;

    engine_ = std::make_unique<RewardsEngineImpl>(std::move(client_remote));

    LOG(ERROR) << "CreateRewardsEngine";

    engine_->Initialize(base::BindOnce(
        &RewardsEngineFactory::OnEngineInitialized,
        base::Unretained(this),
        std::move(engine_receiver),
        std::move(callback)));

    return;

    /*
    engine_ = mojo::MakeSelfOwnedAssociatedReceiver(
        std::make_unique<RewardsEngineImpl>(std::move(client_remote)),
        std::move(engine_receiver));
    */
  }

  std::move(callback).Run(false);
}

void RewardsEngineFactory::OnEngineInitialized(
    mojo::PendingAssociatedReceiver<mojom::RewardsEngine> engine_receiver,
    CreateRewardsEngineCallback callback,
    bool success) {
  DCHECK(engine_);
  LOG(ERROR) << "OnEngineInitialized";
  engine_->Bind(std::move(engine_receiver));
  std::move(callback).Run(success);
}

}  // namespace brave_rewards::internal
