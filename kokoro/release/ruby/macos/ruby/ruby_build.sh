#!/bin/bash

set -ex

# Build protoc
use_bazel.sh 4.0.0
bazel build //:protoc

umask 0022
pushd ruby
bundle update && bundle exec rake gem:native
ls pkg
mv pkg/* $ARTIFACT_DIR
popd
