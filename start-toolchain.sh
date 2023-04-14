#!/bin/bash
set -euo pipefail

git submodule init
git submodule update
cd toolchain
make shell
