#!/usr/bin/env python3
# *******************************************************************************
# Copyright (c) 2026 Accenture
#
# This program and the accompanying materials are made available under the
# terms of the Apache License Version 2.0 which is available at
# https://www.apache.org/licenses/LICENSE-2.0
#
# SPDX-License-Identifier: Apache-2.0
# *******************************************************************************

import os
import subprocess
import sys
import tempfile
from pathlib import Path

WORKSPACE_PATH = "ws/size-delta-source/workspace.josh"
COMPOSE_FILTER = ":+ws/size-delta/s32k148-gcc"


def output(command: list[str], environment: dict[str, str]) -> str:
    return subprocess.check_output(command, env=environment, text=True).strip()


def commit_workspace(
    source_ref: str,
    parent_ref: str,
    workspace_filter: str,
    environment: dict[str, str],
) -> str:
    with tempfile.TemporaryDirectory() as directory:
        index_environment = environment.copy()
        index_environment["GIT_INDEX_FILE"] = str(Path(directory) / "index")
        subprocess.run(["git", "read-tree", source_ref], env=index_environment, check=True)
        blob = subprocess.check_output(
            ["git", "hash-object", "-w", "--stdin"],
            env=index_environment,
            input=workspace_filter,
            text=True,
        ).strip()
        subprocess.run(
            [
                "git",
                "update-index",
                "--add",
                "--cacheinfo",
                f"100644,{blob},{WORKSPACE_PATH}",
            ],
            env=index_environment,
            check=True,
        )
        tree = output(["git", "write-tree"], index_environment)
        return output(
            [
                "git",
                "commit-tree",
                tree,
                "-p",
                parent_ref,
                "-m",
                "Materialize S32K148 size baseline",
            ],
            environment,
        )


def materialize_source(
    input_ref: str,
    parent_ref: str,
    source_filter: str,
    baseline_filter: str,
    environment: dict[str, str],
) -> tuple[str, str]:
    workspace_parent = commit_workspace(
        parent_ref, parent_ref, source_filter, environment
    )
    workspace_tip = commit_workspace(
        input_ref, workspace_parent, baseline_filter, environment
    )
    baseline_source = output(
        [
            "josh-filter",
            ':~(history="no-splice")[:workspace=ws/size-delta-source:exclude[::workspace.josh]]',
            workspace_tip,
        ],
        environment,
    )
    current_source = output(["josh-filter", source_filter, input_ref], environment)
    return baseline_source, current_source


def materialize_compose_workspace(
    input_ref: str, source_ref: str, environment: dict[str, str]
) -> str:
    source_tree = output(["git", "rev-parse", f"{source_ref}^{{tree}}"], environment)
    workspace_filter = (
        f":[:$source={source_tree},::docker/development/,"
        "::ws/common/dev-image.josh,::ws/size-delta/]"
    )
    return output(["josh-filter", workspace_filter, input_ref], environment)


def run_snapshot(ref: str, environment: dict[str, str]) -> Path:
    subprocess.run(
        ["josh", "compose", "run", ref, COMPOSE_FILTER],
        env=environment,
        check=True,
    )
    hashes = output(
        ["josh", "compose", "list-jobs", "--all", ref, COMPOSE_FILTER],
        environment,
    ).splitlines()
    if len(hashes) != 1:
        raise RuntimeError(f"expected one snapshot job, got {len(hashes)}")
    return Path(".josh/success") / hashes[0]


def main() -> None:
    input_ref = sys.argv[1] if len(sys.argv) == 2 else "."
    if len(sys.argv) > 2:
        raise SystemExit(f"usage: {Path(sys.argv[0]).name} [REF]")

    environment = os.environ.copy()
    environment["JOSH_EXPERIMENTAL_FEATURES"] = "1"
    environment.setdefault("GIT_AUTHOR_NAME", "OpenBSW size delta")
    environment.setdefault("GIT_AUTHOR_EMAIL", "openbsw-size-delta@example.invalid")
    environment.setdefault("GIT_COMMITTER_NAME", environment["GIT_AUTHOR_NAME"])
    environment.setdefault("GIT_COMMITTER_EMAIL", environment["GIT_AUTHOR_EMAIL"])
    resolved_input = output(["josh-filter", ":/", input_ref], environment)
    parent_ref = output(["git", "rev-parse", f"{resolved_input}^"], environment)

    directory = Path(__file__).resolve().parent
    source_filter = (directory / "s32k148-gcc-source.josh").read_text()
    baseline_filter = (directory / "s32k148-gcc-baseline.josh").read_text()
    baseline_source, current_source = materialize_source(
        resolved_input,
        parent_ref,
        source_filter,
        baseline_filter,
        environment,
    )
    baseline_ref = materialize_compose_workspace(
        resolved_input, baseline_source, environment
    )
    current_ref = materialize_compose_workspace(
        resolved_input, current_source, environment
    )

    baseline_record = run_snapshot(baseline_ref, environment)
    current_record = run_snapshot(current_ref, environment)
    subprocess.run(
        [
            sys.executable,
            str(directory / "s32k148-gcc.analyze.py"),
            "compare",
            str(baseline_record),
            str(current_record),
        ],
        check=True,
    )


if __name__ == "__main__":
    main()
