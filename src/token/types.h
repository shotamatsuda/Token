// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_TYPES_H_
#define TOKEN_TYPES_H_

namespace token {

enum class Cap {
  UNDEFINED,
  BUTT,
  ROUND,
  PROJECT
};

enum class Join {
  UNDEFINED,
  MITER,
  ROUND,
  BEVEL
};

enum class Align {
  UNDEFINED,
  NONE,
  LEFT,
  RIGHT
};

}  // namespace token

#endif  // TOKEN_TYPES_H_
