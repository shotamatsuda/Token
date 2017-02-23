#!/bin/sh
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

follow_symlink() {
  local file=$1
  local target
  target=$(readlink "${file}")
  if [[ "${target}" ]]; then
    local directory
    directory=$(dirname "${file}")
    file=$(follow_symlink "${directory}/${target}")
  fi
  echo "${file}"
}

code_sign() {
  if [[ -z "${CODE_SIGN_IDENTITY}" ]]; then
    return
  fi
  local file=$1
  local identity="${CODE_SIGN_IDENTITY}"
  if [[ ! -z "${EXPANDED_CODE_SIGN_IDENTITY_NAME}" ]]; then
    identity="${EXPANDED_CODE_SIGN_IDENTITY_NAME}"
  fi
  echo "${identity}"
  codesign --sign "${identity}" "${file}"
}

bundle_dependency() {
  local file
  file=$(follow_symlink "$1")
  local skip_rewrite=$2
  local copy
  copy=$(basename "${file}")
  if [[ ! -e "${file}" ]]; then
    file="${BUILT_PRODUCTS_DIR}/${copy}"
  fi
  if [[ $skip_rewrite == true && -e "${copy}" ]]; then
    return
  fi
  if [[ ! -e "${file}" ]]; then
    return
  fi
  cp -f "${file}" "." || return
  chmod 0755 "${copy}" || return
  code_sign "${copy}"
  local install_name
  install_name=$(otool -D "${file}" | grep -v ":$" | egrep -o "[^/]+$")
  install_name_tool -id "@rpath/${install_name}" "${copy}" || return
  if [[ "${copy}" != "${install_name}" ]]; then
    ln -sf "${copy}" "${install_name}" || return
  fi
  resolve_dependencies "${copy}" true
}

resolve_dependencies() {
  local file=$1
  local skip_rewrite=$2
  otool -L "${file}" | egrep -o "(/usr/local|@rpath).*dylib" | while read; do
    local dependancy
    dependancy=$(basename ${REPLY})
    install_name_tool -change "${REPLY}" "@rpath/${dependancy}" "${file}"
    bundle_dependency "${REPLY}" $skip_rewrite
  done
}

if [[ ! -d "${BUILT_PRODUCTS_DIR}/${FRAMEWORKS_FOLDER_PATH}" ]]; then
  mkdir -p "${BUILT_PRODUCTS_DIR}/${FRAMEWORKS_FOLDER_PATH}"
fi
cd "${BUILT_PRODUCTS_DIR}/${FRAMEWORKS_FOLDER_PATH}"
resolve_dependencies "${BUILT_PRODUCTS_DIR}/${EXECUTABLE_PATH}" false
