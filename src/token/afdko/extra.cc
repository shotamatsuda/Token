//
//  The MIT License
//
//  Copyright (C) 2015-2017 Shota Matsuda
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

#include "token/afdko/extra.h"

#include <cstdlib>
#include <ostream>
#include <sstream>
#include <string>

#include <boost/format.hpp>
#include <boost/filesystem.hpp>

namespace token {
namespace afdko {

bool generateKernFile(const std::string& script, const std::string& input) {
  const auto module = "generateSingleKernFile";
  std::ostringstream oss;
  oss << "export PYTHONPATH=${PYTHONPATH}:'%1%'" << std::endl;
  oss << "python -m '%2%' '%3%'" << std::endl;
  boost::format format(oss.str());
  const auto command = (format % script % module % input).str();
  if (std::system(command.c_str())) {
    return false;
  }
  const boost::filesystem::path path(input);
  boost::filesystem::rename(
      path.parent_path() / ("kern_" + path.stem().string() + ".fea"),
      path.parent_path() / "kern.fea");
  return true;
}

bool generateMarkFile(const std::string& script, const std::string& input) {
  const auto module = "generateSingleMarkFiles";
  std::ostringstream oss;
  oss << "export PYTHONPATH=${PYTHONPATH}:'%1%'" << std::endl;
  oss << "python -m '%2%' '%3%'" << std::endl;
  boost::format format(oss.str());
  const auto command = (format % script % module % input).str();
  if (std::system(command.c_str())) {
    return false;
  }
  const boost::filesystem::path path(input);
  boost::filesystem::rename(
      path.parent_path() / ("mark_" + path.stem().string() + ".fea"),
      path.parent_path() / "mark.fea");
  return true;
}

}  // namespace afdko
}  // namespace token
