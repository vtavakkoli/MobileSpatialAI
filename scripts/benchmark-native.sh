#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD="${ROOT}/build/native-release"
mkdir -p "${ROOT}/results"
cmake -S "${ROOT}/native" -B "${BUILD}" -DCMAKE_BUILD_TYPE=Release -DMSAI_BUILD_TESTS=ON -DMSAI_BUILD_BENCHMARK=ON
cmake --build "${BUILD}" --parallel
ctest --test-dir "${BUILD}" --output-on-failure
"${BUILD}/msai_benchmark" | tee "${ROOT}/results/native-benchmark.json"
