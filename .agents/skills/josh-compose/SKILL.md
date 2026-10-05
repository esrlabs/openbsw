---
name: josh-compose
description: Run and diagnose OpenBSW formatting, builds, and tests with `josh compose`. Use when changing compose.josh, ws/**/*.josh, ws/**/*.run.sh, docker/fmt, docker/development, or when asked to verify the repository workflow.
---

# Josh Compose in OpenBSW

`josh compose` is a containerized build orchestrator based on Josh filters and workspaces. Run it from the repository root. It snapshots the selected Git input, builds the required container images, runs workspace dependencies, and caches successful jobs by the filtered workspace tree hash.

## Prerequisites

- Use the installed `josh` binary. This repository does not contain the Josh CLI source, so do not substitute `cargo run --bin josh`.
- A working Podman installation is required by the current Josh compose backend.
- Network access is required when images or tool downloads are not cached.
- The installed Josh release requires `JOSH_EXPERIMENTAL_FEATURES=1` for every `josh compose` command. Export it once per shell:

  ```sh
  export JOSH_EXPERIMENTAL_FEATURES=1
  ```

Check availability without mutating state:

```sh
josh --version
josh compose --help
```

## Repository workflow graph

The default filter is `:+compose`, which loads `compose.josh`.

`compose.josh` is an orchestrator-only workspace labeled `OpenBSW`. Its `$output="none"` means it records success but does not create an output artifact. It runs these inputs:

| Input | Workspace | Behavior |
|---|---|---|
| `fmt` | `ws/format/format.josh` | Runs `treefmt`, then copies the formatted worktree to `/out`; `$output="workdir"` writes changes back only when the input reference is `.`. |
| `sphinx-docs` | `ws/docs/sphinx.josh` | Builds the Sphinx HTML documentation with warnings treated as errors. |
| `code-coverage` | `ws/code-coverage/report.josh` | Builds the S32K1XX and POSIX debug presets in separate restricted workspaces, then merges their lcov tracefiles, generates HTML and badges, and writes `code_coverage/` back to the working tree. |
| `posix` | `ws/platform/posix.josh` | Builds the `posix-freertos` CMake preset. |
| `clang-tidy-posix` | `ws/clang-tidy/posix.josh` | Builds `tests-posix-release` with Clang 17, mounts the build output, and runs `clang-tidy-17` over a relocated, Josh-filtered analysis worktree. |
| `clang-tidy-s32k1xx` | `ws/clang-tidy/s32k1xx.josh` | Builds `tests-s32k1xx-release` with Clang 17, mounts the build output, and runs `clang-tidy-17` over a relocated, Josh-filtered analysis worktree. |
| `unit-test` | `ws/test/unit.josh` | Builds and runs the `tests-posix-debug` preset; stale coverage `.gcda` files are removed before CTest. |
| `s32k148-gcc` | `ws/platform/s32k148-gcc.josh` | Builds the `s32k148-freertos-gcc` preset with the ARM GNU toolchain and publishes the reference application ELF for dependent jobs. |

The format workspace uses `docker/fmt` through `ws/format/image.josh`. The documentation workspace uses `docker/Dockerfile.docs` through `ws/docs/image.josh`. The build and test workspaces use `docker/development` through `ws/common/dev-image.josh`. Workspace filters deliberately include only the files needed by each job; changing an excluded file does not invalidate that job.

`ws/test/integration.josh`, `ws/platform/s32k148-gdb.josh`, and `ws/size-delta/s32k148-gcc.josh` exist but are not inputs of `compose.josh`, so the default run does not execute them. The `s32k148-gdb` workspace consumes the S32K148 GCC build output and provides commands that operate on the resulting binary artifact. Its `flash` command connects to a separately running native-host GDB server at `host.containers.internal:7224`; the GDB server does not run in a container.

The `s32k148-gcc` size-delta workspace builds filtered baseline and current source trees, records each ELF's ROM, RAM, binary, and allocated-section sizes, then prints their deltas. Without `--arg baseline=...`, the baseline is the selected input's first parent (`{#^}`). An explicit baseline accepts any Git single-revision expression through `--arg baseline=<revision>`.

## Running the workflow

Run the working tree, including uncommitted changes:

```sh
josh compose run
```

Input-reference semantics:

```sh
josh compose run                 # working tree; default; workdir outputs may be extracted
josh compose run --revision +    # Git index only
josh compose run --revision HEAD # committed HEAD; ignores local changes
josh compose run --revision <rev> # any resolvable Git revision
```

The optional positional argument is a Josh filter. Use it to run a single workspace:

```sh
josh compose run :+ws/format/format
josh compose run :+ws/docs/sphinx
josh compose run :+ws/code-coverage/report
josh compose run :+ws/clang-tidy/posix
josh compose run :+ws/clang-tidy/s32k1xx
josh compose run :+ws/platform/posix
josh compose run :+ws/test/unit
josh compose run :+ws/platform/s32k148-gcc
josh compose run :+ws/platform/s32k148-gdb -- flash
josh compose run :+ws/size-delta/s32k148-gcc
josh compose run --arg baseline=origin/main :+ws/size-delta/s32k148-gcc
```

Prefer the narrowest workspace covering the change while iterating. Before delivery, run the default `josh compose run` when the requested acceptance criterion is the complete repository workflow.

For size-delta comparisons, use the default invocation to compare the selected input against its first parent. Use `--arg baseline=<revision>` to compare against a branch, tag, or commit explicitly. `--revision <revision>` changes the current side of the comparison; `--arg baseline=<revision>` changes only the baseline.

## Cache behavior

A job hash is the workspace tree object ID after filtering. Successful and failed result metadata, including captured stdout and stderr, is stored in the repository at `refs/josh/compose`. Results are arranged under `success/<AA>/<BBB>/<REST>` and `failed/<AA>/<BBB>/<REST>`, where the job hash is split across those path components.

A successful `$output="none"` job is skipped when its success record exists. Jobs with outputs are skipped only when both the success record and output volume exist. A normal cache hit looks like:

```text
[OpenBSW] Using cached output (<hash>)
```

Source or workflow changes that affect a filtered workspace produce a new hash and bypass stale results automatically. `--no-distributed-cache` disables distributed filter-cache access; it does not disable compose result reuse from `refs/josh/compose`.

A command supplied after `--` always executes in the selected workspace without replacing its configured cached result or output artifact. Use this for side-effecting operations. The `s32k148-gdb` workspace's default `run` command lists its binary artifact commands; invoke the existing flash action as `josh compose run :+ws/platform/s32k148-gdb -- flash`, ensuring every invocation flashes the board while its firmware build dependency remains cacheable.

Inspect the full dependency plan without executing jobs:

```sh
JOSH_EXPERIMENTAL_FEATURES=1 josh compose list-jobs --all
JOSH_EXPERIMENTAL_FEATURES=1 josh compose list-images --all
```

Omit `--all` to list only work not pruned by cached successful results. Job hashes are dependency-first; image names have the form `josh_ws_image_<hash>`.

Result metadata can be synchronized through Git without transferring runtime output volumes:

```sh
josh compose pull --remote origin
josh compose push --remote origin
```

Both commands default to `origin`. Pull merges remote and local results; push merges concurrent updates before retrying the remote ref update.

## Cleaning and disk reclamation

Do not run `clean` merely to force a workspace to execute. The cache key follows filtered content and invalidates itself. Use a command override after `--` for an intentional uncached action.

Cleaning is a destructive standalone subcommand, not an option to `run`:

```sh
# Remove output artifacts, workspace images, and refs/josh/compose result metadata
josh compose clean

# Also remove persistent josh_cache_* volumes
josh compose clean --all
```

Normal `run` and `shell` operations automatically monitor runtime storage. At 90% usage, compose removes least-recently-used `josh_ws_image_*` images and `josh_out_*` output artifacts until usage is at most 80%, while protecting resources needed by the pending graph. Automatic reclamation does not remove persistent cache volumes, result metadata, or remote objects.

## Reading output and diagnosing failures

Workspace status lines identify the label and job hash:

```text
[unit tests] Running (<hash>)
[unit tests] SUCCESS
[OpenBSW] Done (orchestrator)
```

On failure:

1. Find the first workspace label ending in `FAILED`; sibling inputs may still be attempted before the orchestrator returns failure.
2. Use the stdout and stderr emitted by the failed run. The same captured streams are recorded with the failed result under `refs/josh/compose`.
3. Re-run only that workspace filter while fixing it.
4. Fix the source, runner, workspace filter, or image definition that owns the failure. Do not delete success records to mask an incorrect filter.
5. Re-run the default workflow after the focused workspace succeeds.

A failure before a workspace status line usually belongs to filter resolution, image construction, or the Podman backend. A failure after `Running` belongs to the workspace command. The default command is `bash run.sh`; `$cmd` overrides it.

## Editing workspace definitions

Reuse the existing structure:

```text
:$label="human-readable label"
:#image[:+ws/common/dev-image]
:$output="none"
:$network="host"

worktree = :[
    :+ws/common/openbsw
    ::path/needed/by/the/job/
    :/ws/<group>::run.sh=<runner>.run.sh
]
```

Key rules:

- Keep `worktree` minimal but complete. Missing headers, sources, CMake files, scripts, or tool data cause isolated-container failures; unrelated files reduce cache precision.
- Map the runner to `/worktree/run.sh` with `:/ws/<group>::run.sh=<runner>.run.sh` unless `$cmd` intentionally replaces the default.
- Use `inputs` for dependency workspaces. Dependencies execute depth-first and non-`none` outputs are mounted read-only at `/<input-name>`.
- Use `$output="none"` for validation-only jobs, `$output="workdir"` only when results must be copied back to the working tree, and the default kept output for artifacts consumed by dependencies.
- Use `$network="host"` only for jobs that need network access. The runtime default is no network.
- Image filters expose a `context` tree consumed as the container build context.
- Architecture-specific downloaded binaries in `docker/fmt/Dockerfile` must select by `TARGETARCH` and verify per-architecture SHA-256 checksums. Keep unsupported architectures as a hard error.

After changing a workspace filter, verify that `josh compose list-jobs --all` resolves the graph, run the affected workspace, then run the default composition when required.
