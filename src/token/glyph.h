//
//  token/glyph.h
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
#include <iostream>
#include <locale>
#include <string>

#include "solas.h"
#include "takram/graphics.h"
#include "takram/math.h"
#include "takram/nanovg.h"
#include "token/glyph_outline.h"
#include "token/glyph_stroker.h"
#include "token/ufo.h"

class App : public solas::View {
 public:
  void setup() override {
    glyphs_ = token::ufo::Glyphs("/Users/sgss/Dropbox/Github/token/Token.ufo");
    auto fontinfo = token::ufo::Fontinfo("/Users/sgss/Dropbox/Github/token/Token.ufo");
    std::cout << fontinfo.family_name << std::endl;
    context_.init();
    scale_ = 1.0;
    width_ = 3;
//    resize(1280, 1024);
  }

  void pre() override {
    context_.begin(width(), height(), scale());
  }

  void post() override {
    context_.end();
  }

  void update() override {
    if (needs_stroking_) {
      token::GlyphStroker stroker;
      stroker.set_width(width_);
      auto shape = glyph_.shape();
      const auto advance = glyph_.glyph().advance->width;
      const auto scale = (680.0 - stroker.width()) / 680.0;
      const takram::Vec2d center(advance / 2.0, 340.0);
      for (auto& command : shape) {
        command.point() = center + (command.point() - center) * scale;
        command.control1() = center + (command.control1() - center) * scale;
        command.control2() = center + (command.control2() - center) * scale;
      }
      shape_ = stroker.stroke(shape);
      shape_ = stroker.simplify(shape_);
      shape_.convertConicsToQuadratics();
      shape_.convertQuadraticsToCubics();
      shape_.removeDuplicates(1.0);
      needs_stroking_ = false;
    }
  }

  void draw() override {
    if (glyph_.glyph().name.empty()) {
      return;
    }
    takram::Shape2d shape = shape_;
    for (auto& command : shape) {
      command.point().y = 1000.0 - command.point().y;
      command.control1().y = 1000.0 - command.control1().y;
      command.control2().y = 1000.0 - command.control2().y;
      command.point() *= scale_;
      command.control1() *= scale_;
      command.control2() *= scale_;
    }
    takram::nvg::Scope save;
    takram::nvg::translate(translation_);
    drawShape(shape);
    takram::nvg::fillColor(takram::Color4d::black());
    takram::nvg::fill();
    for (const auto& path : shape.paths()) {
      takram::nvg::beginPath();
      drawPath(path);
      if (path.direction() == takram::PathDirection::CLOCKWISE) {
        takram::nvg::strokeColor(takram::Color4d::cyan());
      } else {
        takram::nvg::strokeColor(takram::Color4d::magenta());
      }
      takram::nvg::stroke();
    }
    drawControl(shape);

    takram::nvg::translateY(-translation_.y);
    takram::nvg::strokeColor(takram::Color4d(0.5, 0.5));
    takram::nvg::beginPath();
    takram::nvg::moveTo(0.0, 0.0);
    takram::nvg::lineTo(0.0, height());
    takram::nvg::stroke();
    const auto advance = glyph_.glyph().advance->width * scale_;
    takram::nvg::beginPath();
    takram::nvg::moveTo(advance, 0.0);
    takram::nvg::lineTo(advance, height());
    takram::nvg::stroke();
  }

  void drawPath(const takram::Path2d& path) {
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
  }

  void drawShape(const takram::Shape2d& shape) {
    takram::nvg::beginPath();
    for (const auto& path : shape.paths()) {
      drawPath(path);
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
    const auto character = std::toupper(key(), std::locale());
    auto name = std::string() + character;
    switch (character) {
      case '1': name = "one"; break;
      case '2': name = "two"; break;
      case '3': name = "three"; break;
      case '4': name = "four"; break;
      case '5': name = "five"; break;
      case '6': name = "six"; break;
      case '7': name = "seven"; break;
      case '8': name = "eight"; break;
      case '9': name = "nine"; break;
      case '0': name = "zero"; break;
      default: break;
    }
    auto glyph = glyphs_.find(name);
    if (glyph) {
      glyph_ = token::GlyphOutline(*glyph);
      needs_stroking_ = true;
    }
  }

  void mouseDragged() override {
    translation_ += mouse() - pmouse();
  }

  void mouseWheel(const solas::MouseEvent& event) override {
    if (event.modifiers() % solas::KeyModifier::ALTERNATE) {
      width_ = std::max(4.0, width_ + event.wheel().y / 2.0);
      needs_stroking_ = true;
    } else {
      const auto scale = scale_;
      scale_ = scale_ * (event.wheel().y < 0.0 ? 0.8 : 1.25);
      const auto delta = mouse() - translation_;
      translation_ += (delta * (scale - scale_)) / scale;
    }
  }

 private:
  token::ufo::Glyphs glyphs_;
  token::GlyphOutline glyph_;
  takram::Shape2d shape_;
  takram::nvg::Context context_;
  takram::Vec2d translation_;
  double width_;
  double scale_;
  bool needs_stroking_;
};
