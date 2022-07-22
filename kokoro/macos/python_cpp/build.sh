#!/bin/bash
#
# Build file to set up and run tests

# Change to repo root
cd $(dirname $0)/../../..

# Prepare worker environment to run tests
KOKORO_INSTALL_VENV=yes
source kokoro/macos/prepare_build_macos_rc

bazel test //python/... //conformance:python_cpp -k --macos_minimum_os=10.9 --test_output=streamed --test_env=PROTOCOL_BUFFERS_PYTHON_IMPLEMENTATION=cpp --define=use_fast_cpp_protos=true