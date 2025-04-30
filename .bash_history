python3 .ci/format.py
python3 .ci/format.py
python3 .ci/format.py
clear
python3 .ci/format.py
exit
python3 .ci/build.py "posix" "gcc" "14"
ln --symbolic cmake-build-posix-gcc cmake-build-posix
python3 .ci/pytest.py
