#!/usr/bin/env bash
set -euo pipefail

BIN=${BUILD_DIR}/runtime

ssh ${REMOTE_HOST} <<EOF
  set -e
  ${BIN}
EOF