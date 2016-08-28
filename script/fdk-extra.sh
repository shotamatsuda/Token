#!/bin/sh
#
#  setup.sh
#
#  The MIT License
#
#  Copyright (C) 2015-2016 Shota Matsuda
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

readonly PROJECT_DIR="$(cd "$(dirname "$0")/../"; pwd)"
readonly TARGET_DIR="${PROJECT_DIR}/build/fdk-extra/src"
readonly BUILD_DIR="${PROJECT_DIR}/build/fdk-extra"
readonly INSTALL_DIR="$1"

# Collect all of the libraries into the target directory
mkdir -p "${TARGET_DIR}"
cp -r "${PROJECT_DIR}/lib/fonttools/Lib/" "${TARGET_DIR}"
cp -r "${PROJECT_DIR}/lib/robofab/Lib/" "${TARGET_DIR}"
cp -r "${PROJECT_DIR}/lib/defcon/Lib/" "${TARGET_DIR}"
cp "${PROJECT_DIR}/lib/python-modules/"*.py "${TARGET_DIR}"
cp "${PROJECT_DIR}/lib/python-scripts/"*.py "${TARGET_DIR}"

# Zip the target directory as an executable
cd "${TARGET_DIR}"
zip -r "${BUILD_DIR}/fdk-extra" *
if [[ ! -d "${INSTALL_DIR}" ]]; then
  mkdir -p "${INSTALL_DIR}"
fi
mv "${BUILD_DIR}/fdk-extra.zip" "${INSTALL_DIR}/fdk-extra"
chmod 0755 "${INSTALL_DIR}/fdk-extra"
