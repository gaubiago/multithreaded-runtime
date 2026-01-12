#!/usr/bin/env bash
set -euo pipefail

ssh ${REMOTE_HOST} <<EOF
  set -e
  mkdir -p ${BUILD_DIR}
  cd ${BUILD_DIR}

  cmake .. -DCMAKE_BUILD_TYPE=Release
  cmake --build . -j\$(nproc)
EOF