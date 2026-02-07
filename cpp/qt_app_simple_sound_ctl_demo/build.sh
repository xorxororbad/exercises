#!/usr/bin/env bash

export SCRIPT_PATH=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)

pushd ${SCRIPT_PATH}/.build
    cmake ..;
    cmake --build .;
    cmake --install . --prefix ../.deploy;
popd

