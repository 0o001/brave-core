/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/body_sniffer/body_sniffer_throttle.h"

#include <utility>

#include "brave/components/body_sniffer/body_sniffer_url_loader.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "services/network/public/mojom/url_response_head.mojom.h"

namespace body_sniffer {

BodySnifferThrottle::BodySnifferThrottle(
    scoped_refptr<base::SequencedTaskRunner> task_runner)
    : task_runner_(std::move(task_runner)) {}

BodySnifferThrottle::~BodySnifferThrottle() = default;

void BodySnifferThrottle::SetBodyProducer(
    std::unique_ptr<class BodyProducer> producer) {
  producer_ = std::move(producer);
  body_handlers_.clear();
}

void BodySnifferThrottle::AddHandler(std::unique_ptr<BodyHandler> handler) {
  if (!producer_) {
    body_handlers_.push_back(std::move(handler));
  }
}

void BodySnifferThrottle::WillStartRequest(network::ResourceRequest* request,
                                           bool* defer) {
  for (auto& handler : body_handlers_) {
    if (!handler->OnRequest(request)) {
      handler.reset();
    }
  }
  std::erase_if(body_handlers_, [](auto& h) { return !h; });
}

void BodySnifferThrottle::WillProcessResponse(
    const GURL& response_url,
    network::mojom::URLResponseHead* response_head,
    bool* defer) {
  for (auto& handler : body_handlers_) {
    bool d = false;
    if (!handler->ShouldProcess(response_url, response_head, &d)) {
      handler.reset();
    } else {
      *defer = *defer || d;
      handler->UpdateResponseHead(response_head);
    }
  }
  std::erase_if(body_handlers_, [](auto& h) { return !h; });
  if (body_handlers_.empty() && !producer_) {
    return;
  }

  Handler handler = std::move(body_handlers_);
  if (producer_) {
    if (response_head) {
      producer_->UpdateResponseHead(response_head);
    }
    handler = std::move(producer_);
    *defer = true;
  }

  mojo::PendingRemote<network::mojom::URLLoader> new_remote;
  mojo::PendingReceiver<network::mojom::URLLoaderClient> new_receiver;
  BodySnifferURLLoader* url_loader = nullptr;
  mojo::ScopedDataPipeConsumerHandle body;

  std::tie(new_remote, new_receiver, url_loader, body) =
      BodySnifferURLLoader::CreateLoader(weak_factory_.GetWeakPtr(),
                                         response_head->Clone(),
                                         std::move(handler), task_runner_);
  InterceptAndStartLoader(std::move(new_remote), std::move(new_receiver),
                          url_loader, std::move(body));
}

void BodySnifferThrottle::InterceptAndStartLoader(
    mojo::PendingRemote<network::mojom::URLLoader> new_remote,
    mojo::PendingReceiver<network::mojom::URLLoaderClient> new_receiver,
    BodySnifferURLLoader* loader,
    mojo::ScopedDataPipeConsumerHandle body) {
  mojo::PendingRemote<network::mojom::URLLoader> source_loader;
  mojo::PendingReceiver<network::mojom::URLLoaderClient> source_client_receiver;

  delegate_->InterceptResponse(std::move(new_remote), std::move(new_receiver),
                               &source_loader, &source_client_receiver, &body);
  loader->Start(std::move(source_loader), std::move(source_client_receiver),
                std::move(body));
}

void BodySnifferThrottle::Cancel() {
  delegate_->CancelWithError(net::ERR_ABORTED);
}

void BodySnifferThrottle::Resume() {
  delegate_->Resume();
}

}  // namespace body_sniffer
