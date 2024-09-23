#! /bin/sh

set -xe

CC="gcc"
FLAGS="-Wall -Wpedantic -ggdb"
SRCS="$(find ./src -type f -name "*.c")"
CURRENT_DIR="$(git rev-parse --show-toplevel)"
BUILD_DIR="${CURRENT_DIR}/build"

mkdir -p ${BUILD_DIR}

${CC} ${FLAGS} -o ${BUILD_DIR}/cdo ${SRCS}
