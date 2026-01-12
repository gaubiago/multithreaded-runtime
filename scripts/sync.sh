#!/usr/bin/env bash
set -euo pipefail

rsync -az --delete \
  --exclude build \
  --exclude .git \
  --exclude .vscode \
  ./ \
  ${REMOTE_HOST}:${REMOTE_DIR}
