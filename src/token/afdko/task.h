// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

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

// MARK: -

// MARK: Modifiers

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
