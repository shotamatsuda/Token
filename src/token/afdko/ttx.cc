//
//  token/afdko/ttx.cc
//
//  The MIT License
//
//  Copyright (C) 2015-2016 Shota Matsuda
//
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.
//

#include "token/afdko/ttx.h"

#include <string>

#include <boost/filesystem/path.hpp>
#include <boost/format.hpp>

namespace token {
namespace afdko {

bool ttx(const std::string& tools, const std::string& input) {
  const auto name = "ttx";
  const auto command = (boost::filesystem::path(tools) / name).string();
  const std::string format = R"(
    export PATH=${PATH}:"%1%"
    export FDK_EXE="%1%"
    "%2%" -f "%3%"
  )";
  return !std::system((
      boost::format(format) %
      tools %
      command %
      input).str().c_str());
}

}  // namespace afdko
}  // namespace token
