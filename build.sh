#!/bin/bash

echo BUILDING PROJECT

set -e

BUILD_DIR="build/unix"

mkdir -p "$BUILD_DIR"

cmake -S . -B "$BUILD_DIR"
cmake --build "$BUILD_DIR"

echo BUILD COMPLETE