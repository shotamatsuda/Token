//
//  The MIT License
//
//  Copyright (C) 2015-2017 Shota Matsuda
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

#include <utility>

#include "shotamatsuda/graphics.h"
#include "token/ufo/font_info.h"
#include "token/ufo/glif/advance.h"
#include "token/ufo/glyph.h"

namespace token {

namespace shota = shotamatsuda;

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

  enum class Align {
    UNDEFINED,
    LEFT,
    RIGHT
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
  std::pair<shota::Shape2d, ufo::glif::Advance> operator()(
      const ufo::FontInfo& font_info,
      const ufo::Glyph& glyph,
      const GlyphOutline& outline) const;

  // Parameters
  double width() const { return width_; }
  void set_width(double value) { width_ = value; }
  double miter() const { return miter_; }
  void set_miter(double value) { miter_ = value; }
  Cap cap() const { return cap_; }
  void set_cap(Cap value) { cap_ = value; }
  Join join() const { return join_; }
  void set_join(Join value) { join_ = value; }
  Align align() const { return align_; }
  void set_align(Align value) { align_ = value; }
  bool filled() const { return filled_; }
  void set_filled(bool value) { filled_ = value; }
  double precision() const { return precision_; }
  void set_precision(double value) { precision_ = value; }
  double shift_increment() const { return shift_increment_; }
  void set_shift_increment(double value) { shift_increment_ = value; }
  double shift_limit() const { return shift_limit_; }
  void set_shift_limit(double value) { shift_limit_ = value; }

 private:
  shota::Shape2d stroke(const ufo::Glyph& glyph,
                        const GlyphOutline& outline) const;
  shota::Shape2d stroke(const GlyphOutline& outline) const;
  shota::Shape2d stroke(const shota::Path2d& path) const;
  shota::Shape2d simplify(const shota::Shape2d& shape) const;

 private:
  double width_;
  double miter_;
  Cap cap_;
  Join join_;
  Align align_;
  bool filled_;
  double precision_;
  double shift_increment_;
  double shift_limit_;
};

// MARK: -

inline GlyphStroker::GlyphStroker()
    : width_(),
      miter_(),
      cap_(Cap::ROUND),
      join_(Join::ROUND),
      align_(Align::UNDEFINED),
      filled_(),
      precision_(1.0),
      shift_increment_(0.0001),
      shift_limit_(0.1) {}

// MARK: Comparison

inline bool operator==(const GlyphStroker& lhs, const GlyphStroker& rhs) {
  return (lhs.width_ == rhs.width_ &&
          lhs.miter_ == rhs.miter_ &&
          lhs.cap_ == rhs.cap_ &&
          lhs.join_ == rhs.join_ &&
          lhs.align_ == rhs.align_ &&
          lhs.filled_ == rhs.filled_ &&
          lhs.precision_ == rhs.precision_ &&
          lhs.shift_increment_ == rhs.shift_increment_ &&
          lhs.shift_limit_ == rhs.shift_limit_);
}

inline bool operator!=(const GlyphStroker& lhs, const GlyphStroker& rhs) {
  return !(lhs == rhs);
}

}  // namespace token

#endif  // TOKEN_GLYPH_STROKER_H_
