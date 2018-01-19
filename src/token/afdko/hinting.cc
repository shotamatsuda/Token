// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#include "token/afdko/hinting.h"

#include <string>

#include "token/afdko/task.h"

namespace token {
namespace afdko {

bool checkOutlines(const std::string& directory,
                   const std::string& input) {
  Task task;
  task.set_directory(directory);
  task.set_name("checkOutlinesUFO");
  task.set_arguments({"-e", "-all", "-decimal", input});
  return task.execute();
}

bool performAutoHinting(const std::string& directory,
                        const std::string& input) {
  Task task;
  task.set_directory(directory);
  task.set_name("autohint");
  task.set_arguments({"-all", "-decimal", input});
  return task.execute();
}

}  // namespace afdko
}  // namespace token
