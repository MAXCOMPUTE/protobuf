#!/bin/bash

set -ex

docker run -it -v$(realpath $(dirname $0)/../..):/workspace $1 "composer test && composer test_c"
