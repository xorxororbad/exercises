#!/usr/bin/env bash

export SCRIPT_PATH="$(cd "$(dirname "$0")"; pwd)";
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${SCRIPT_PATH}/.deploy/lib/"

pushd "${SCRIPT_PATH}/.deploy/bin/"
    ./qt_app_simple_sount_ctl_demo_exec
popd
