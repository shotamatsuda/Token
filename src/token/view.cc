//
//  token/view.cc
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

#include "token/view.h"

#include <cassert>
#include <string>
#include <utility>
#include <vector>

#include "takram/graphics.h"
#include "takram/nanovg.h"

namespace token {

void View::setup() {
  context_.init();
  scale_ = 1.0;
  stroker_.set_width(50.0);
  needs_stroking_ = true;

  font_info_.open("/Users/sgss/Dropbox/Github/token/Token.ufo");
  glyphs_.open("/Users/sgss/Dropbox/Github/token/Token.ufo");

  names_ = {
    "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O",
    "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "one", "two",
    "three", "four", "five", "six", "seven", "eight", "nine", "zero",
  };
  for (const auto& name : names_) {
    outlines_.emplace(name, GlyphOutline(glyphs_.get(name)));
  }
}

void View::pre() {
  context_.begin(width(), height(), scale());
}

void View::post() {
  context_.end();
}

void View::update() {
  if (!needs_stroking_) {
    return;
  }
  shapes_.clear();
  for (const auto& name : names_) {
    takram::Shape2d shape = outlines_.at(name).shape();
    const auto advance = glyphs_.get(name).advance->width;
    const auto scale = (font_info_.ascender - stroker_.width()) / font_info_.ascender;
    const takram::Vec2d center(advance / 2.0, 340.0);
    for (auto& command : shape) {
      command.point() = center + (command.point() - center) * scale;
      command.control1() = center + (command.control1() - center) * scale;
      command.control2() = center + (command.control2() - center) * scale;
    }
    shape = stroker_.stroke(shape);
    shape = stroker_.simplify(shape);
    shape.convertConicsToQuadratics();
    shape.convertQuadraticsToCubics();
    shape.removeDuplicates(1.0);
    shapes_.emplace(name, std::move(shape));
  }
  needs_stroking_ = false;
}

void View::draw() {
  takram::nvg::Scope save;
  takram::nvg::fillColor(takram::Color4d::white());
  takram::nvg::beginPath();
  takram::nvg::rect(size());
  takram::nvg::fill();
  takram::nvg::translate(translation_);
  takram::nvg::save();
  auto itr = std::begin(names_);
  for (int i{}; itr != std::end(names_); ++itr, ++i) {
    if (!(i % 13)) {
      takram::nvg::restore();
      takram::nvg::translateY(font_info_.units_per_em * scale_);
      takram::nvg::save();
    }
    takram::nvg::beginPath();
    drawShape(shapes_.at(*itr));
    takram::nvg::fillColor(takram::Color4d::black());
    takram::nvg::fill();
    takram::nvg::translateX(glyphs_.get(*itr).advance->width * scale_);
  }
}

void View::mouseDragged() {
  translation_ += mouse() - pmouse();
}

void View::mouseWheel(const solas::MouseEvent& event) {
  if (event.modifiers() % solas::KeyModifier::ALTERNATE) {
    stroker_.set_width(takram::math::constrain(
        stroker_.width() + event.wheel().y, 10.0, 120.0));
    needs_stroking_ = true;
  } else {
    const auto scale = scale_;
    scale_ = scale_ * (event.wheel().y < 0.0 ? 0.8 : 1.25);
    const auto delta = mouse() - translation_;
    translation_ += (delta * (scale - scale_)) / scale;
  }
}

void View::drawShape(const takram::Shape2d& shape) {
  for (const auto& path : shape.paths()) {
    drawPath(path);
    if (path.direction() == takram::PathDirection::CLOCKWISE) {
      takram::nvg::pathWinding(NVG_SOLID);
    } else {
      takram::nvg::pathWinding(NVG_HOLE);
    }
  }
}

void View::drawPath(const takram::Path2d& path) {
  for (const auto& command : path) {
    switch (command.type()) {
      case takram::graphics::CommandType::MOVE:
        takram::nvg::moveTo(
            command.point().x * scale_,
            (font_info_.units_per_em - command.point().y) * scale_);
        break;
      case takram::graphics::CommandType::LINE:
        takram::nvg::lineTo(
            command.point().x * scale_,
            (font_info_.units_per_em - command.point().y) * scale_);
        break;
      case takram::graphics::CommandType::QUADRATIC:
        takram::nvg::quadTo(
            command.control().x * scale_,
            (font_info_.units_per_em - command.control().y) * scale_,
            command.point().x * scale_,
            (font_info_.units_per_em - command.point().y) * scale_);
        break;
      case takram::graphics::CommandType::CUBIC:
        takram::nvg::cubicTo(
            command.control1().x * scale_,
            (font_info_.units_per_em - command.control1().y) * scale_,
            command.control2().x * scale_,
            (font_info_.units_per_em - command.control2().y) * scale_,
            command.point().x * scale_,
            (font_info_.units_per_em - command.point().y) * scale_);
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

}  // namespace token
