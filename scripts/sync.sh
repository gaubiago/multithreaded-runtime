#!/usr/bin/env bash
set -euo pipefail

rsync -az --delete \
  --exclude build \
  --exclude .git \
  ./ \
  ${REMOTE_HOST}:${REMOTE_DIR}
