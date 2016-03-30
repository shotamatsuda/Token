//
//  token/afdko/task.h
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

#pragma once
#ifndef TOKEN_AFDKO_TASK_H_
#define TOKEN_AFDKO_TASK_H_

#include <string>
#include <vector>

namespace token {
namespace afdko {

class Task final {
 public:
  Task() = default;

  // Copy semantics
  Task(const Task&) = default;
  Task& operator=(const Task&) = default;

  // Executing task
  bool execute();

  // Modifiers
  const std::string& directory() const;
  void set_directory(const std::string& value);
  const std::string& name() const;
  void set_name(const std::string& value);
  const std::vector<std::string>& arguments() const;
  void set_arguments(const std::vector<std::string>& value);

 private:
  std::string directory_;
  std::string name_;
  std::vector<std::string> arguments_;
};

#pragma mark -

#pragma mark Modifiers

inline const std::string& Task::directory() const {
  return directory_;
}

inline void Task::set_directory(const std::string& value) {
  directory_ = value;
}

inline const std::string& Task::name() const {
  return name_;
}

inline void Task::set_name(const std::string& value) {
  name_ = value;
}

inline const std::vector<std::string>& Task::arguments() const {
  return arguments_;
}

inline void Task::set_arguments(const std::vector<std::string>& value) {
  arguments_ = value;
}

}  // namespace afdko
}  // namespace token

#endif  // TOKEN_AFDKO_TASK_H_
