// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_AFDKO_HINTING_H_
#define TOKEN_AFDKO_HINTING_H_

#include <string>

namespace token {
namespace afdko {

bool checkOutlines(const std::string& directory,
                   const std::string& input);
bool performAutoHinting(const std::string& directory,
                        const std::string& input);

}  // namespace afdko
}  // namespace token

#endif  // TOKEN_AFDKO_HINTING_H_
