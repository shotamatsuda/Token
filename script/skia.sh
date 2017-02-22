#!/bin/sh
#
#  skia.sh
#
#  The MIT License
#
#  Copyright (C) 2015-2017 Shota Matsuda
#
#  Permission is hereby granted, free of charge, to any person obtaining a
#  copy of this software and associated documentation files (the "Software"),
#  to deal in the Software without restriction, including without limitation
#  the rights to use, copy, modify, merge, publish, distribute, sublicense,
#  and/or sell copies of the Software, and to permit persons to whom the
#  Software is furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
#  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#  DEALINGS IN THE SOFTWARE.
#

readonly DEPOT_TOOLS_GIT="https://chromium.googlesource.com/chromium/tools/depot_tools.git"
readonly SKIA_GIT="https://skia.googlesource.com/skia.git"

readonly PROJECT_DIR="$(cd "$(dirname "$0")/../"; pwd)"
readonly BUILD_DIR="${PROJECT_DIR}/build/skia"
readonly DEPOT_TOOLS_DIR="${BUILD_DIR}/depot_tools"
readonly SKIA_DIR="${BUILD_DIR}/skia"

cleanup() {
  echo "Cleaning everything before we start to build..."
  rm -r "${SKIA_DIR}/out"
  rm -r "${BUILD_DIR}/lib"
  rm -r "${BUILD_DIR}/include"
}

download_depot_tools() {
  if [[ ! -d "${DEPOT_TOOLS_DIR}" ]]; then
    echo "Downloading depot tools..."
    git clone "${DEPOT_TOOLS_GIT}" "${DEPOT_TOOLS_DIR}"
  fi
}

download_skia() {
  if [[ ! -d "${SKIA_DIR}" ]]; then
    mkdir -p "${SKIA_DIR}"
  fi
  echo "Downloading skia..."
  git clone "${SKIA_GIT}" "${SKIA_DIR}"
  cd "${SKIA_DIR}"
  python "./tools/git-sync-deps"
}

build_skia() {
  export PATH="${DEPOT_TOOLS_DIR}:${PATH}"
  cd "${SKIA_DIR}"
  echo "Building for OS X..."
  export MACOSX_DEPLOYMENT_TARGET=10.11
  "./bin/gn" gen "out/Release" --args='is_debug=false'
  ninja -C "out/Release" skia
  copy_libraries_in_place "${SKIA_DIR}/out/Release"
}

copy_libraries_in_place() {
  local dir=$1
  echo "Copying libraries in place..."
  mkdir -p "${BUILD_DIR}/lib"
  for file in $(find "${dir}" -name "*.a"); do
    cp -n "${file}" "${BUILD_DIR}/lib"
  done
}

decompose_libraries() {
  echo "Decomposing libraries to object files..."
  mkdir -p "${BUILD_DIR}/lib/obj"
  for file in $(find "${BUILD_DIR}/lib" -name "*.a"); do
    cd "${BUILD_DIR}/lib/obj"; ar -x "${file}"
  done
}

create_library() {
  echo "Linking everything within architectures..."
  cd "${BUILD_DIR}/lib"
  xcrun ar crus "libskia.a" obj/*.o
  rm -r "${BUILD_DIR}/lib/obj"

  echo "Copying headers..."
  cd "${SKIA_DIR}/include"
  for header in $(find . -name "*.h"); do
    ditto "${header}" "${BUILD_DIR}/include/${header}"
  done
  cd "${SKIA_DIR}/src"
  for header in $(find . -name "*.h"); do
    ditto "${header}" "${BUILD_DIR}/include/${header}"
  done
}

cleanup
download_depot_tools
download_skia
build_skia
decompose_libraries
create_library
