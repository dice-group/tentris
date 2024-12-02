#!/bin/bash

set -euo pipefail

VERSION_REGEX='project\([^)]*VERSION\s+(\d+\.\d+.\d+)[^)]*\)'
TENTRIS_VER=$(grep -Poz "$VERSION_REGEX" CMakeLists.txt | grep -Poz '[0-9]+\.[0-9]+\.[0-9]+')
CUR_BRANCH=$(git branch --show-current | sed 's|/|_|g')
# get current commit hash
COMMIT_HASH=$(git rev-parse --short HEAD)
if [[ -n $CUR_BRANCH ]]; then
    TENTRIS_NAME="tentris_${TENTRIS_VER}_${CUR_BRANCH}_${COMMIT_HASH}"
else
    TENTRIS_NAME="tentris_${TENTRIS_VER}_${COMMIT_HASH}"
fi

if podman --version > /dev/null; then
    echo "Using podman"
    BUILDER="podman"
elif docker --version > /dev/null; then
    echo "Using docker"
    BUILDER="docker"
else
    echo "Error: Neither podman nor docker found" 2>&1
    exit 1
fi

cleanup_and_tmpdir() {
    TEMP_DIR=$(mktemp -d) || exit 1
}

# make sure TEMP_DIR will be removed on failure
trap 'rm -rf "$TEMP_DIR"' EXIT

cleanup_and_tmpdir

mkdir -p "${TENTRIS_NAME}"
${BUILDER} build . --output="${TEMP_DIR}" --target=binaries
echo "${TEMP_DIR}"
ls -lah "${TEMP_DIR}"
cp -r "${TEMP_DIR}/." "./${TENTRIS_NAME}/"