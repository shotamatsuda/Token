//
//  token/stroker.h
//
//  The MIT License
//
//  Copyright (C) 2015 Shota Matsuda
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
#ifndef TOKEN_STROKER_H_
#define TOKEN_STROKER_H_

#include "takram/graphics/shape.h"

namespace token {

class Stroker final {
 public:
  using Real = double;

 public:
  enum class Cap {
    BUTT,
    ROUND,
    PROJECT
  };

  enum class Join {
    MITER,
    ROUND,
    BEVEL
  };

 public:
  Stroker();

  // Copy semantics
  Stroker(const Stroker& other) = default;
  Stroker& operator=(const Stroker& other) = default;

  // Stroking
  takram::Shape2d operator()(const takram::Shape2d& shape) const;

  // Comparison
  bool operator==(const Stroker& other) const;
  bool operator!=(const Stroker& other) const;

  // Parameters
  Real width() const { return width_; }
  void set_width(Real value) { width_ = value; }
  Real miter() const { return miter_; }
  void set_miter(Real value) { miter_ = value; }
  Cap cap() const { return cap_; }
  void set_cap(Cap value) { cap_ = value; }
  Join join() const { return join_; }
  void set_join(Join value) { join_ = value; }
  Real tolerance() const { return tolerance_; }
  void set_tolerance(Real value) { tolerance_ = value; }

 private:
  Real width_;
  Real miter_;
  Cap cap_;
  Join join_;
  Real tolerance_;
};

#pragma mark -

inline Stroker::Stroker()
    : width_(),
      miter_(),
      cap_(Cap::BUTT),
      join_(Join::MITER),
      tolerance_(1.0) {}

#pragma mark Comparison

inline bool Stroker::operator==(const Stroker& other) const {
  return (width_ == other.width_ &&
          miter_ == other.miter_ &&
          cap_ == other.cap_ &&
          join_ == other.join_ &&
          tolerance_ == other.tolerance_);
}

inline bool Stroker::operator!=(const Stroker& other) const {
  return !operator==(other);
}

}  // namespace token

#endif  // TOKEN_STROKER_H_
