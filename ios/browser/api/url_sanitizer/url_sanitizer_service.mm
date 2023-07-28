/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/url_sanitizer/browser/url_sanitizer_service.h"
#include "brave/ios/browser/api/url_sanitizer/url_sanitizer_service+private.h"

@interface URLSanitizerService () {
  brave::URLSanitizerService* urlSanitizer_;
}

@end

@implementation URLSanitizerService

- (instancetype)initWithURLSanitizerService:
    (brave::URLSanitizerService*)urlSanitizer {
  self = [super init];
  if (self) {
    urlSanitizer_ = urlSanitizer;
  }
  return self;
}

- (NSURL*)sanitizedURL:(NSURL*)url {
  return url;
}

@end
