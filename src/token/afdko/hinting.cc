//
//  token/afdko/hinting.cc
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
