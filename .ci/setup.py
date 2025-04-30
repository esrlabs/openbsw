import os
import subprocess
import shutil
import sys

platforms = ["posix", "s32k148"]
compilers = ["clang", "gcc"]
cpp_standards = ["14", "17", "20", "23"]

def get_full_command_path(command):
    if (cmd := shutil.which(command)) is None:
        print(f"ERROR: Compiler command {command} not found!")
        sys.exit(1)
    return cmd

def get_environment_variables(platform, compiler):
    env = dict(os.environ)
    if platform == "s32k148" and compiler == "gcc":
        env["CC"] = get_full_command_path("arm-none-eabi-gcc")
        env["CXX"] = get_full_command_path("arm-none-eabi-gcc")
    elif platform == "posix" and compiler == "clang":
        env["CC"] = get_full_command_path("clang")
        env["CXX"] = get_full_command_path("clang++")
    elif platform == "s32k148" and compiler == "clang":
        env["CC"] = get_full_command_path("/usr/bin/llvm-arm/bin/clang")
        env["CXX"] = get_full_command_path("/usr/bin/llvm-arm/bin/clang++")
    elif platform == "posix" and compiler == "gcc":
        env["CC"] = get_full_command_path("gcc")
        env["CXX"] = get_full_command_path("gcc")
    return env

def main():
    for platform in platforms:
        for compiler in compilers:
            for cpp_standard in cpp_standards:
                print(f"Running build for platform={platform}, compiler={compiler}, cpp_standard={cpp_standard}")
                subprocess.run(
                    ["python3", ".ci/build.py", platform, compiler, str(cpp_standard)],
                    env=get_environment_variables(platform, compiler),
                    check=True
                )
    print("All combinations have been processed.")

if __name__ == "__main__":
    main()