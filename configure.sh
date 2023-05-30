#!/bin/bash

BUILD_TYPE=${1:-Release}
BUILD_DIR=build-${BUILD_TYPE}

mkdir -p ${BUILD_DIR}
if cmake -S . -B ${BUILD_DIR} -DCMAKE_BUILD_TYPE=${BUILD_TYPE}; then
    echo "CMake configure finished"
else
    exit $?
fi
