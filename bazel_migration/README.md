# Bazel migration for OpenBSW


## Status

- POC `arm-none-eabi-gcc` toolchain for `s32k148` target
- Toolchain reproducibility is provided by the Docker container (pins `arm-none-eabi-gcc` at a fixed version under `/opt/arm-gnu-toolchain`). This does not represent full hermeticity and would break cache correctness in remote cache scenarios.
- Build output for one example library (`libs/bsw/util`) verified against CMake output.
- Conditional dependency selection via `label_flag` (`etl_profile`) for build variants (`reference_app` / `unit_test`)
- No further libraries or executables have been migrated yet.

Open:
- Bazel readme and integration guide
- Bazel CI tests
- Clang toolchain
- Consider if toolchain should be made fully hermetic
- Migration of all libs and executables
- Migration of unit test configs

```
OpenBSW Bazel migration
├── bazel/ ✅ (toolchain arm-none-eabi-gcc for s32k148)
├── cmake/ ⬛
├── doc/ ⬛
├── docker/ ⬛
├── executables/
│   ├── referenceApp/ 🔲
│   └── unitTest/ 🔲
├── libs/
│   ├── 3rdparty/
│   │   └── etl ✅
│   ├── bsw/
│   │   ├── platform ✅
│   │   └── util ✅
│   └── (remaining) 🔲
├── platforms/
│   ├── posix/ 🔲
│   └── s32k1xx/ 🔲
├── test/ Scope of Bazel support TBD
└── tools/ Scope of Bazel support TBD

✅ done
🔲 todo
⬛ not applicable
```

## Quick start

```bash
# Build all targets for s32k148:
bazel build --config=s32k148 //...
# Build example target for s32k148:
bazel build --config=s32k148 //libs/bsw/util:util
# Run unit tests for s32k148:
bazel test --config=s32k148 //...
```

## Build Configuration

Implemented config points:

| Config point | Explanation | CLI | Bazel mechanism | Values | Default |
|---|---|---|---|---|---|
| [`platform`](../bazel/platform/BUILD) | Selects toolchain based on target platform | `--config=s32k148` | `platform` | `//bazel/platform:s32k148`, `@platforms//host` | `@platforms//host` |
| [`executable_config`](../bazel/config/executable_config/BUILD) | Controls executable config; `unit_test` is incompatible with baremetal platforms (e.g `s32k148`) | `--//bazel/config/executable_config` | `string_flag` | `reference_app`, `unit_test` | `reference_app`, `unit_test` (bazel test invocations) |
| [`etl_profile`](../libs/3rdparty/etl/BUILD) | Injects custom ETL profile; otherwise use default based on executable_config | `--//libs/3rdparty/etl:etl_profile` | `label_flag` | any `cc_library` label | `//executables/referenceApp/etl_profile` (`executable_config:reference_app`), `//executables/unitTest/etl_profile` (`executable_config:unit_test`) |

Examples:
```bash
# Inject a custom ETL profile
bazel build --config=s32k148 --//libs/3rdparty/etl:etl_profile=//custom/path:my_profile //libs/3rdparty/etl:etl
```

## Toolchain verification
This directory contains build artifact comparisons for the the [s32k148 POC Bazel toolchain](../bazel/toolchain/BUILD) based on the following example:
- `libs/bsw/util` library and its dependencies:
    - `libs/3rdparty/etl`
    - `libs/bsw/platform`

### Analysis scope

The following properties were compared between the CMake (`s32k148-freertos-gcc / RelWithDebInfo`) and Bazel (`s32k148_relwithdebinfo`) builds:

1. **File size**
2. **Translation units**: Comparison of compiled source files
3. **Symbol table**: Comparison of full symbol table after filtering out debug level information (`wm4.*` GCC debug path markers, `$d`/`$t` ARM section markers).

### Files
- [`artifact_comparison_method.md`](artifact_comparison_method.md): Commands used to perform the analysis
- [`artifact_comparison__util.md`](artifact_comparison__util.md): Comparison results for `libs/bsw/util`