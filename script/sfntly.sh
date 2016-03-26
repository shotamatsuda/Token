#!/bin/sh
#
#  sfntly.sh
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
readonly TARGET_DIR="${PROJECT_DIR}/lib/sfntly/cpp"
readonly BUILD_DIR="${PROJECT_DIR}/build/sfntly"

# System's ICU doesn't supply headers, so change the cmake lists to link
# against Homebrew's installation.
sed -i .backup '21i\
include_directories(/usr/local/opt/icu4c/include)\
link_directories(/usr/local/opt/icu4c/lib)\
' "${TARGET_DIR}/CMakeLists.txt"

# Clone the newest google test framework, as the one sfntly provides is ancient
# and doesn't build.
cd "${TARGET_DIR}/ext"
git clone git@github.com:google/googletest.git
mv "googletest/googletest" "gtest"
rm -rf "googletest"

# Make
mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"
cmake "${TARGET_DIR}"
make

# Clean up
rm -rf "${TARGET_DIR}/ext/gtest"
rm "${TARGET_DIR}/CMakeLists.txt"
mv "${TARGET_DIR}/CMakeLists.txt.backup" "${TARGET_DIR}/CMakeLists.txt"
