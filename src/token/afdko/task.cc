//
//  token/afdko/task.cc
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

#include "token/afdko/task.h"

#include <cstdlib>
#include <ostream>
#include <sstream>
#include <string>

#include <boost/algorithm/string/join.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/format.hpp>

namespace token {
namespace afdko {

#pragma mark Executing task

bool Task::execute() {
  const auto path = (boost::filesystem::path(directory_) / name_).string();
  const auto arguments = boost::join(arguments_, " ");
  std::ostringstream oss;
  oss << "export PATH=${PATH}:'%1%'" << std::endl;
  oss << "export FDK_EXE='%1%'" << std::endl;
  oss << "'%2%' %3%" << std::endl;
  boost::format format(oss.str());
  const auto command = (format % directory_ % path % arguments).str();
  return !std::system(command.c_str());
}

}  // namespace afdko
}  // namespace token