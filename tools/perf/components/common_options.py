# Copyright (c) 2023 The Brave Authors. All rights reserved.
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at https://mozilla.org/MPL/2.0/.

import argparse
import logging
import sys
import tempfile

from typing import Optional, List

import components.path_util as path_util

with path_util.SysPath(path_util.GetTelemetryDir()):
  with path_util.SysPath(path_util.GetChromiumPerfDir()):
    from core.perf_benchmark import PerfBenchmark  # pylint: disable=import-error


class CommonOptions:
  verbose: bool = False
  ci_mode: bool = False
  variations_repo_dir: Optional[str] = None
  working_directory: str = ''
  target_os: str = PerfBenchmark.FixupTargetOS(sys.platform)

  do_run_tests: bool = True
  do_report: bool = False
  report_on_failure: bool = False
  local_run: bool = False
  compare = False
  targets: List[str] = []
  config: str = ''

  @classmethod
  def add_parser_args(cls, parser: argparse.ArgumentParser) -> None:
    parser.add_argument('--verbose', action='store_true')
    parser.add_argument('--ci-mode', action='store_true')
    parser.add_argument('--variations-repo-dir', type=str)
    parser.add_argument('--working-directory', type=str)
    parser.add_argument('--target_os', type=str)
    parser.add_argument('--no-report', action='store_true')
    parser.add_argument('--report-only', action='store_true')
    parser.add_argument('--report-on-failure', action='store_true')
    parser.add_argument('--local-run', action='store_true')
    parser.add_argument('--compare', action='store_true')
    parser.add_argument('--targets',
                      required=True,
                      type=str,
                      help='Tags/binaries to test')
    parser.add_argument('--config', required=True, type=str)

  @classmethod
  def from_args(cls, args) -> 'CommonOptions':
    options = CommonOptions()
    options.verbose = args.verbose
    options.ci_mode = args.ci_mode
    options.variations_repo_dir = args.variations_repo_dir
    if args.working_directory is None:
      if options.ci_mode:
        raise RuntimeError(f'Set --working-directory for --ci-mode')
      else:
        options.working_directory = tempfile.mkdtemp(prefix='perf-test-')
    else:
      options.working_directory = args.working_directory
    if args.target_os is not None:
      options.target_os = PerfBenchmark.FixupTargetOS(args.target_os)

    options.do_run_tests = not args.report_only
    options.do_report = not args.no_report and not args.local_run
    options.report_on_failure = args.report_on_failure
    options.local_run = args.local_run
    options.targets = args.targets.split(',')
    if args.targets == 'compare' or args.compare:
      options.compare = options.local_run = True

    return options
