import shutil
from pathlib import Path
import subprocess
import sys

def configure_and_build(platform, compiler, cpp_standard):
    build_dir = Path(f"cmake-build-{platform}-{compiler}")
    if build_dir.exists():
        shutil.rmtree(build_dir)

    cmake_command = [
        "cmake",
        "-B", build_dir,
        "-S", "executables/referenceApp",
        f"-DCMAKE_CXX_STANDARD={cpp_standard}",
        f"-DBUILD_TARGET_PLATFORM={'POSIX' if platform == 'posix' else 'S32K148EVB'}",
        f"-DCMAKE_TOOLCHAIN_FILE={'../../admin/cmake/ArmNoneEabi-' + compiler + '.cmake' if platform == 's32k148' else ''}"
    ]
    subprocess.run(cmake_command, check=True)
    subprocess.run(["cmake", "--build", build_dir, "--target", "app.referenceApp", "-j"], check=True)

def main():
    if len(sys.argv) != 4:
        print("ERROR: Usage: build.py <platform> <compiler> <cpp_standard>")
        sys.exit(1)

    platform = sys.argv[1]
    compiler = sys.argv[2]
    cpp_standard = sys.argv[3]

    configure_and_build(platform, compiler, cpp_standard)

if __name__ == "__main__":
    main()
