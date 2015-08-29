#!/bin/sh
#
#  afdko.sh
#
#  The MIT License
#
#  Copyright (C) 2015 Shota Matsuda
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
readonly BUILD_DIR="${PROJECT_DIR}/build/afdko"
readonly AFDKO_ARCHIVE="${BUILD_DIR}/afdko.zip"

download_afdko() {
  if [[ ! -d "${BUILD_DIR}" ]]; then
    mkdir -p "${BUILD_DIR}"
  fi
  if [[ ! -s "${AFDKO_ARCHIVE}" ]]; then
    echo "Downloading AFDKO"
    curl -L -o "${AFDKO_ARCHIVE}" \
        "https://github.com/adobe-type-tools/afdko/releases/download/2.5.64655/FDK-2.5.64655-MAC.zip"
  fi
}

extract_afdko() {
  if [[ ! -f "${AFDKO_ARCHIVE}" ]]; then
    echo "Source archive is missing."
    exit
  fi
  echo "Extracting boost into ${BOOST_DIR}..."
  if [[ ! -d "${BOOST_DIR}" ]]; then
    mkdir -p "${BOOST_DIR}"
    tar -xjf "${BOOST_ARCHIVE}" -C "${BOOST_DIR}" --strip-components=1
  fi
  if [[ -d "${BOOST_DIR}" ]]; then
    echo "Unpacked as ${BOOST_DIR}"
  fi
}
