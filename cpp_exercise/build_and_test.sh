#!/usr/bin/sh
set -x

echo "Trying to build \"ipcreceiver.cc\" and \"ipcsender.cc\" using build \n"

bazel build --cxxopt='-std=c++14' //src/main:ipcsender
bazel build --cxxopt='-std=c++14' //src/main:ipcreceiver

echo "Running the Test Cases\n"

bazel test --cxxopt='-std=c++14' //tests:linuxipc_test

rm -r -f binary_output
rm -r -f test_output

mkdir binary_output
mkdir test_output

cp bazel-bin/src/main/ipcreceiver binary_output
cp bazel-bin/src/main/ipcsender binary_output
cp bazel-testlogs/tests/linuxipc_test/test.log test_output

bazel clean