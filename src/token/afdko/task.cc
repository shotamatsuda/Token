// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

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

// MARK: Executing task

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
