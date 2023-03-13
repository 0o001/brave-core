
/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_ads/core/internal/common/url/url_request_string_util.h"

#include <sstream>
#include <vector>

#include "base/ranges/algorithm.h"
#include "base/strings/string_util.h"
#include "base/strings/stringprintf.h"
#include "brave/components/brave_ads/common/interfaces/ads.mojom.h"

namespace brave_ads {

namespace {

bool ShouldAllowHeader(const std::string& header) {
  const std::vector<std::string> allowed_headers{"digest", "signature",
                                                 "accept", "content-type"};

  return base::ranges::any_of(
      allowed_headers, [&header](const std::string& allowed_header) {
        return base::StartsWith(header, allowed_header,
                                base::CompareCase::INSENSITIVE_ASCII);
      });
}

std::string HeadersToString(const std::vector<std::string>& headers,
                            const int indent = 4) {
  std::vector<std::string> formatted_headers;

  const std::string spaces = std::string(indent, ' ');

  for (const auto& header : headers) {
    if (!ShouldAllowHeader(header)) {
      continue;
    }

    const std::string formatted_header =
        base::StringPrintf("%s%s", spaces.c_str(), header.c_str());

    formatted_headers.push_back(formatted_header);
  }

  return base::JoinString(formatted_headers, "\n");
}

}  // namespace

std::string UrlRequestToString(const mojom::UrlRequestInfoPtr& url_request) {
  std::string log = "URL Request:\n";

  log += base::StringPrintf("  URL: %s\n", url_request->url.spec().c_str());

  if (!url_request->content.empty()) {
    log += base::StringPrintf("  Content: %s\n", url_request->content.c_str());
  }

  if (!url_request->content_type.empty()) {
    log += base::StringPrintf("  Content Type: %s\n",
                              url_request->content_type.c_str());
  }

  std::ostringstream ss;
  ss << url_request->method;

  log += base::StringPrintf("  Method: %s", ss.str().c_str());

  return log;
}

std::string UrlRequestHeadersToString(
    const mojom::UrlRequestInfoPtr& url_request) {
  std::string log = "  Headers:\n";

  if (!url_request->headers.empty()) {
    log += HeadersToString(url_request->headers);
  }

  return log;
}

}  // namespace brave_ads
