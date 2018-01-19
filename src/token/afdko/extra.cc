// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

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
