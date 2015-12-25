//
//  token/glyph_stroker.h
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
#ifndef TOKEN_GLYPH_STROKER_H_
#define TOKEN_GLYPH_STROKER_H_

#include "takram/graphics.h"

namespace token {

class GlyphOutline;

class GlyphStroker final {
 public:
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

 public:
  GlyphStroker();

  // Copy semantics
  GlyphStroker(const GlyphStroker&) = default;
  GlyphStroker& operator=(const GlyphStroker&) = default;

  // Comparison
  friend bool operator==(const GlyphStroker& lhs, const GlyphStroker& rhs);
  friend bool operator!=(const GlyphStroker& lhs, const GlyphStroker& rhs);

  // Stroking
  takram::Shape2d stroke(const GlyphOutline& outline) const;
  takram::Shape2d stroke(const takram::Shape2d& shape) const;
  takram::Shape2d stroke(const takram::Path2d& path) const;
  takram::Shape2d simplify(const takram::Shape2d& shape) const;

  // Parameters
  double width() const { return width_; }
  void set_width(double value) { width_ = value; }
  double miter() const { return miter_; }
  void set_miter(double value) { miter_ = value; }
  Cap cap() const { return cap_; }
  void set_cap(Cap value) { cap_ = value; }
  Join join() const { return join_; }
  void set_join(Join value) { join_ = value; }
  double precision() const { return precision_; }
  void set_precision(double value) { precision_ = value; }
  double tolerance() const { return tolerance_; }
  void set_tolerance(double value) { tolerance_ = value; }

 private:
  takram::Shape2d merge(const takram::Shape2d& shape) const;

 private:
  double width_;
  double miter_;
  mutable Cap cap_;
  Join join_;
  double precision_;
  double tolerance_;
};

#pragma mark -

inline GlyphStroker::GlyphStroker()
    : width_(),
      miter_(),
      cap_(Cap::ROUND),
      join_(Join::ROUND),
      precision_(0.25),
      tolerance_(0.02) {}

#pragma mark Comparison

inline bool operator==(const GlyphStroker& lhs, const GlyphStroker& rhs) {
  return (lhs.width_ == rhs.width_ &&
          lhs.miter_ == rhs.miter_ &&
          lhs.cap_ == rhs.cap_ &&
          lhs.join_ == rhs.join_ &&
          lhs.precision_ == rhs.precision_ &&
          lhs.tolerance_ == rhs.tolerance_);
}

inline bool operator!=(const GlyphStroker& lhs, const GlyphStroker& rhs) {
  return !(lhs == rhs);
}

}  // namespace token

#endif  // TOKEN_GLYPH_STROKER_H_
