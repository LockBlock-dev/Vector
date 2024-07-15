#!/bin/bash

if [ "$1" == "clean" ]; then
    rm -r build
    exit 0
fi

mkdir -p build

build_folder=${1:-release}
build_type=$(echo "$build_folder" | awk '{print toupper(substr($0, 1, 1)) tolower(substr($0, 2))}')

if [ "$build_type" != "Release" ] && [ "$build_type" != "Debug" ]; then
    echo "Invalid build type '$build_type'. Must be 'release' or 'debug'."
    exit 1
fi

cmake -S . -B "build/$build_folder" -DCMAKE_BUILD_TYPE=$build_type
cmake --build "build/$build_folder"
