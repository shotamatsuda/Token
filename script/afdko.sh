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

readonly BUILD_DIR="${PROJECT_DIR}/build"
readonly AFDKO_DIR="${BUILD_DIR}/afdko"
readonly AFDKO_ARCHIVE="${BUILD_DIR}/afdko.zip"
readonly INSTALL_DIR="${BUILT_PRODUCTS_DIR}/${SHARED_SUPPORT_FOLDER_PATH}";

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
  if [[ ! -d "${AFDKO_DIR}" ]]; then
    echo "Extracting AFDKO into ${AFDKO_DIR}..."
    mkdir -p "${AFDKO_DIR}"
    unzip -q "${AFDKO_ARCHIVE}" -d "${AFDKO_DIR}"
    rm -rf "${AFDKO_DIR}/__MACOSX"
    if [[ -d "${AFDKO_DIR}" ]]; then
      echo "Unpacked as ${AFDKO_DIR}"
    fi
  fi
}

install_afdko() {
  if [[ ! -d "${INSTALL_DIR}/FDK" ]]; then
    if [[ ! -d "${INSTALL_DIR}" ]]; then
      mkdir -p "${INSTALL_DIR}"
    fi
    cp -r "${AFDKO_DIR}/FDK" "${INSTALL_DIR}/FDK"
  fi
}

download_afdko
extract_afdko
install_afdko
