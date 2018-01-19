// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_AFDKO_EXTRA_H_
#define TOKEN_AFDKO_EXTRA_H_

#include <string>

namespace token {
namespace afdko {

bool generateKernFile(const std::string& script, const std::string& input);
bool generateMarkFile(const std::string& script, const std::string& input);

}  // namespace afdko
}  // namespace token

#endif  // TOKEN_AFDKO_EXTRA_H_
