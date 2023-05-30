#!/bin/bash

BUILD_TYPE=${1:-Release}
BUILD_DIR=build-${BUILD_TYPE}

if cmake --build ${BUILD_DIR}; then
    echo "CMake build finished"
else
    exit $?
fi
