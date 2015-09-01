//
//  token/view.h
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
#ifndef TOKEN_VIEW_H_
#define TOKEN_VIEW_H_

#include <map>
#include <string>

#include "solas.h"
#include "takram/graphics.h"
#include "takram/nanovg.h"
#include "token/glyph_outline.h"
#include "token/glyph_stroker.h"
#include "token/ufo.h"

namespace token {

class View : public solas::View {
 public:
  // Lifecycle
  void setup() override;
  void pre() override;
  void post() override;
  void update() override;
  void draw() override;

  // Events
  void mouseDragged() override;
  void mouseWheel(const solas::MouseEvent& event) override;

 private:
  void drawShape(const takram::Shape2d& shape);
  void drawPath(const takram::Path2d& path);

 private:
  takram::nvg::Context context_;
  token::ufo::FontInfo font_info_;
  token::ufo::Glyphs glyphs_;
  token::GlyphStroker stroker_;
  std::vector<std::string> names_;
  std::map<std::string, token::GlyphOutline> outlines_;
  std::map<std::string, takram::Shape2d> shapes_;
  takram::Vec2d translation_;
  double scale_;
  bool needs_stroking_;
};

}  // namespace token

#endif  // TOKEN_VIEW_H_
