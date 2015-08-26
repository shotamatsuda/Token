//
//  main.cc
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

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>

#include "solas.h"
#include "takram/graphics.h"
#include "takram/math.h"
#include "takram/nanovg.h"
#include "token/glyph_outline.h"
#include "token/ufo/glif.h"
#include "token/ufo/glyphs.h"

class App : public solas::View {
 public:
  void setup() override {
    glyphs_ = token::ufo::Glyphs("/Users/sgss/Desktop/AkkuratStd-Regular.ufo/glyphs/");
    context_.init();
    scale_ = 1.0;
    resize(1280, 1024);
  }

  void pre() override {
    context_.begin(width(), height(), scale());
  }

  void post() override {
    context_.end();
  }

  void draw() override {
    takram::Shape2d shape = glyph_.shape();
    for (auto& command : shape) {
      command.point() *= scale_;
      command.control1() *= scale_;
      command.control2() *= scale_;
    }
    takram::nvg::Scope save;
    takram::nvg::translate(translation_);
    takram::nvg::beginPath();
    takram::nvg::translate(0, height());
    takram::nvg::scale(1.0, -1.0);
    drawShape(shape);
    takram::nvg::fillColor(takram::Color4d(0.0));
    takram::nvg::fill();
    drawControl(shape);
  }

  void drawShape(const takram::Shape2d& shape) {
    for (const auto& path : shape.paths()) {
      for (const auto& command : path) {
        switch (command.type()) {
          case takram::graphics::CommandType::MOVE:
            takram::nvg::moveTo(command.point());
            break;
          case takram::graphics::CommandType::LINE:
            takram::nvg::lineTo(command.point());
            break;
          case takram::graphics::CommandType::QUADRATIC:
            takram::nvg::quadTo(command.control(), command.point());
            break;
          case takram::graphics::CommandType::CUBIC:
            takram::nvg::cubicTo(command.control1(),
                                 command.control2(),
                                 command.point());
            break;
          case takram::graphics::CommandType::CLOSE:
            takram::nvg::closePath();
            break;
          default:
            assert(false);
            break;
        }
      }
      if (path.direction() == takram::PathDirection::CLOCKWISE) {
        takram::nvg::pathWinding(NVG_SOLID);
      } else {
        takram::nvg::pathWinding(NVG_HOLE);
      }
    }
  }

  void drawControl(const takram::Shape2d& shape) {
    takram::nvg::Scope save;
    takram::nvg::beginPath();
    for (const auto& command : shape) {
      switch (command.type()) {
        case takram::graphics::CommandType::MOVE:
          takram::nvg::moveTo(command.point());
          break;
        case takram::graphics::CommandType::LINE:
          takram::nvg::lineTo(command.point());
          break;
        case takram::graphics::CommandType::QUADRATIC:
          takram::nvg::lineTo(command.control());
          takram::nvg::lineTo(command.point());
          break;
        case takram::graphics::CommandType::CUBIC:
          takram::nvg::lineTo(command.control1());
          takram::nvg::lineTo(command.control2());
          takram::nvg::lineTo(command.point());
          break;
        case takram::graphics::CommandType::CLOSE:
          takram::nvg::closePath();
          break;
        default:
          break;
      }
    }
    takram::nvg::strokeWidth(1.0);
    takram::nvg::strokeColor(takram::Color4d(0.5, 0.5));
    takram::nvg::stroke();
    takram::nvg::fillColor(takram::Color4d(0.5));
    takram::nvg::strokeColor(takram::Color4d(0.5));
    const takram::Size2d size(4.0);
    for (const auto& command : shape) {
      switch (command.type()) {
        case takram::graphics::CommandType::MOVE:
          takram::nvg::beginPath();
          takram::nvg::rect(command.point() - size / 2.0, size);
          takram::nvg::fill();
          break;
        case takram::graphics::CommandType::LINE:
          takram::nvg::beginPath();
          takram::nvg::rect(command.point() - size / 2.0, size);
          takram::nvg::fill();
          break;
        case takram::graphics::CommandType::QUADRATIC:
          takram::nvg::beginPath();
          takram::nvg::rect(command.control() - size / 2.0, size);
          takram::nvg::stroke();
          takram::nvg::beginPath();
          takram::nvg::rect(command.point() - size / 2.0, size);
          takram::nvg::stroke();
          takram::nvg::fill();
          break;
        case takram::graphics::CommandType::CUBIC:
          takram::nvg::beginPath();
          takram::nvg::rect(command.control1() - size / 2.0, size);
          takram::nvg::stroke();
          takram::nvg::beginPath();
          takram::nvg::rect(command.control2() - size / 2.0, size);
          takram::nvg::stroke();
          takram::nvg::beginPath();
          takram::nvg::rect(command.point() - size / 2.0, size);
          takram::nvg::stroke();
          takram::nvg::fill();
          break;
        default:
          break;
      }
    }
  }

  void keyPressed() override {
    auto& glyph = glyphs_.get(std::string() + key());
    if (glyph) {
      glyph_ = token::GlyphOutline(*glyph);
    }
  }

  void mouseDragged() override {
    translation_ += mouse() - pmouse();
  }

  void mouseWheel(const solas::MouseEvent& event) override {
    const auto scale = scale_;
    scale_ = std::max(0.5, scale_ * (event.wheel().y < 0.0 ? 0.8 : 1.25));
    const auto delta = mouse() - translation_;
    translation_ += (delta * (scale - scale_)) / scale;
  }

 private:
  token::ufo::Glyphs glyphs_;
  token::GlyphOutline glyph_;
  takram::nvg::Context context_;
  takram::Vec2d translation_;
  double scale_;
};

int main(int argc, char **argv) {
  solas::RunOptions options;
  options.runner().set_backend(solas::Backend::OPENGL3 |
                               solas::Backend::OPENGLES3);
  return solas::run<App>(argc, argv, options);
}
