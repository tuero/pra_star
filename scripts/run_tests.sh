#!/bin/bash
cd "$(dirname "$0")"
cd ../build
ctest -j$(nproc) --output-on-failure

