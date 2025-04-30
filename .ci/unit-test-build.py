import subprocess
from pathlib import Path
import shutil

def configure_cmake(build_type):

    unit_tests_build_dir = Path("cmake-build-unit-tests")
    if unit_tests_build_dir.exists():
      shutil.rmtree(unit_tests_build_dir)

    subprocess.run([
        "cmake", "-B", "cmake-build-unit-tests", "-S", "executables/unitTest",
        "-DBUILD_UNIT_TESTS=ON",
        f"-DCMAKE_BUILD_TYPE={build_type}",
        "-DCMAKE_C_COMPILER_LAUNCHER=sccache",
        "-DCMAKE_CXX_COMPILER_LAUNCHER=sccache"
    ], check=True)

def build_with_cmake():
    subprocess.run(["cmake", "--build", "cmake-build-unit-tests", "-j4"], check=True)

def run_tests():
    subprocess.run(["ctest", "--test-dir", "cmake-build-unit-tests", "-j4"], check=True)

if __name__ == "__main__":
    configure_cmake("Debug")
    build_with_cmake()
    run_tests()