/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/reduce_language/browser/reduce_language_component_installer.h"

#include <string>

#include "base/base_paths.h"
#include "base/command_line.h"
#include "base/containers/flat_set.h"
#include "base/json/json_reader.h"
#include "base/logging.h"
#include "base/task/thread_pool.h"
#include "base/types/expected.h"
#include "brave/components/brave_component_updater/browser/dat_file_util.h"
#include "brave/components/brave_component_updater/browser/local_data_files_service.h"
#include "net/base/registry_controlled_domains/registry_controlled_domain.h"

using brave_component_updater::LocalDataFilesObserver;
using brave_component_updater::LocalDataFilesService;

namespace reduce_language {

const char kReduceLanguageConfigFile[] = "reduce-language.json";
const char kReduceLanguageConfigFileVersion[] = "1";

ReduceLanguageComponentInstallerPolicy::ReduceLanguageComponentInstallerPolicy(
    LocalDataFilesService* local_data_files_service)
    : LocalDataFilesObserver(local_data_files_service) {}

ReduceLanguageComponentInstallerPolicy::~ReduceLanguageComponentInstallerPolicy() =
    default;

void ReduceLanguageComponentInstallerPolicy::LoadDirectlyFromResourcePath() {
  base::FilePath dat_file_path =
      resource_dir_.AppendASCII(kReduceLanguageConfigFile);
  base::ThreadPool::PostTaskAndReplyWithResult(
      FROM_HERE, {base::MayBlock()},
      base::BindOnce(&brave_component_updater::GetDATFileAsString,
                     dat_file_path),
      base::BindOnce(&ReduceLanguageComponentInstallerPolicy::OnDATFileDataReady,
                     weak_factory_.GetWeakPtr()));
}

void ReduceLanguageComponentInstallerPolicy::AddObserver(Observer* observer) {
  observers_.AddObserver(observer);
}

void ReduceLanguageComponentInstallerPolicy::RemoveObserver(Observer* observer) {
  observers_.RemoveObserver(observer);
}

void ReduceLanguageComponentInstallerPolicy::OnDATFileDataReady(
    const std::string& contents) {
  for (Observer& observer : observers_) {
    observer.OnRulesReady(contents);
  }
}

void ReduceLanguageComponentInstallerPolicy::OnComponentReady(
    const std::string& component_id,
    const base::FilePath& install_dir,
    const std::string& manifest) {
  resource_dir_ = install_dir.AppendASCII(kReduceLanguageConfigFileVersion);
  LoadDirectlyFromResourcePath();
}

}  // namespace reduce_language
