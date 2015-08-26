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

#include <vector>

#include "solas.h"
#include "takram/graphics.h"
#include "takram/math.h"
#include "takram/nanovg.h"

#include "token/pathfinder.h"
#include "token/stroker.h"

namespace token {

class App : public solas::View {
 public:
  void setup() override {
    context_.init();
    width_ = 5.0;
    scale_ = 1.0;
    draws_control_ = true;
    draws_outline_ = false;

    shape_.moveTo(142.3, -303.9);
    shape_.cubicTo(142.3, -303.9, 155.3, -307, 155.3, -307);
    shape_.cubicTo(155.3, -307, 155.3, -375, 155.3, -375);
    shape_.cubicTo(155.3, -375, 142.3, -378.1, 142.3, -378.1);
    shape_.moveTo(37.7, -303.9);
    shape_.cubicTo(37.7, -303.9, 24.7, -307, 24.7, -307);
    shape_.cubicTo(24.7, -307, 24.7, -375, 24.7, -375);
    shape_.cubicTo(24.7, -375, 37.7, -378.1, 37.7, -378.1);
    shape_.moveTo(291.6, -307);
    shape_.cubicTo(291.6, -307, 248.4, -375, 248.4, -375);
    shape_.moveTo(270, -310.4);
    shape_.cubicTo(254.6, -310.4, 246.4, -324.1, 246.4, -341);
    shape_.cubicTo(246.4, -357.9, 254.6, -371.6, 270, -371.6);
    shape_.cubicTo(285.4, -371.6, 293.6, -357.9, 293.6, -341);
    shape_.cubicTo(293.6, -324.1, 285.4, -310.4, 270, -310.4);
    shape_.moveTo(-330, -73.8);
    shape_.cubicTo(-330, -73.8, -330, -67, -330, -67);
    shape_.moveTo(-330, -135);
    shape_.cubicTo(-330, -135, -330, -91.3, -330, -91.3);
    shape_.moveTo(-101.9, -91.3);
    shape_.cubicTo(-101.9, -91.3, -78.1, -91.3, -78.1, -91.3);
    shape_.moveTo(-90, -135);
    shape_.cubicTo(-90, -135, -90, -67, -90, -67);
    shape_.moveTo(-460.4, -91.3);
    shape_.cubicTo(-460.4, -91.3, -436.6, -91.3, -436.6, -91.3);
    shape_.moveTo(-436.6, -67);
    shape_.cubicTo(-443, -67, -450, -70, -450, -84.7);
    shape_.cubicTo(-450, -84.7, -450, -135, -450, -135);
    shape_.moveTo(-239.9, -90);
    shape_.cubicTo(-239.9, -90, -239.9, -107.7, -239.9, -107.7);
    shape_.moveTo(-210, -107.7);
    shape_.cubicTo(-210, -95, -202.2, -90, -195, -90);
    shape_.cubicTo(-187.8, -90, -180, -95, -180, -107.7);
    shape_.cubicTo(-180, -107.7, -180, -135, -180, -135);
    shape_.moveTo(-239.9, -135);
    shape_.cubicTo(-239.9, -135, -239.9, -107.7, -239.9, -107.7);
    shape_.cubicTo(-239.9, -95, -232.1, -90, -224.9, -90);
    shape_.cubicTo(-217.8, -90, -209.9, -95, -210, -107.7);
    shape_.cubicTo(-210, -107.7, -210, -135, -210, -135);
    shape_.moveTo(607.2, -375);
    shape_.cubicTo(607.2, -375, 652.8, -307, 652.8, -307);
    shape_.moveTo(607.2, -307);
    shape_.cubicTo(607.2, -307, 652.8, -375, 652.8, -375);
    shape_.moveTo(607.2, -307);
    shape_.cubicTo(607.2, -307, 607.2, -375, 607.2, -375);
    shape_.cubicTo(607.2, -375, 652.8, -375, 652.8, -375);
    shape_.cubicTo(652.8, -375, 652.8, -307, 652.8, -307);
    shape_.cubicTo(652.8, -307, 607.2, -307, 607.2, -307);
    shape_.moveTo(605.3, 173);
    shape_.cubicTo(605.3, 173, 654.7, 173, 654.7, 173);
    shape_.moveTo(630, 173);
    shape_.cubicTo(630, 173, 630, 105, 630, 105);
    shape_.moveTo(-342.8, -341);
    shape_.cubicTo(-342.8, -341, -317.2, -341, -317.2, -341);
    shape_.moveTo(-90, -371.3);
    shape_.cubicTo(-91, -371.3, -91.8, -372.2, -91.8, -373.2);
    shape_.cubicTo(-91.8, -374.2, -91, -375, -90, -375);
    shape_.cubicTo(-89, -375, -88.2, -374.2, -88.2, -373.2);
    shape_.cubicTo(-88.2, -372.2, -89, -371.3, -90, -371.3);
    shape_.moveTo(-188.4, -307);
    shape_.cubicTo(-188.4, -307, -231.6, -375, -231.6, -375);
    shape_.moveTo(-355.5, 53);
    shape_.cubicTo(-355.5, 53, -330, -15, -330, -15);
    shape_.cubicTo(-330, -15, -304.5, 53, -304.5, 53);
    shape_.moveTo(-187.4, 231.2);
    shape_.cubicTo(-191.9, 226.9, -197.3, 224.3, -205.4, 224.3);
    shape_.cubicTo(-222.8, 224.3, -232.6, 239.8, -232.6, 259);
    shape_.cubicTo(-232.6, 278.2, -222.8, 293.7, -205.4, 293.7);
    shape_.cubicTo(-197.3, 293.7, -191.9, 291.1, -187.4, 286.8);
    shape_.moveTo(-349.5, 173);
    shape_.cubicTo(-349.5, 173, -349.5, 105, -349.5, 105);
    shape_.cubicTo(-349.5, 105, -310.5, 105, -310.5, 105);
    shape_.moveTo(-427.1, 105);
    shape_.cubicTo(-427.1, 105, -457.9, 145, -457.9, 145);
    shape_.moveTo(-472.9, 173);
    shape_.cubicTo(-472.9, 173, -472.9, 105, -472.9, 105);
    shape_.moveTo(-472.9, 130.1);
    shape_.cubicTo(-472.9, 130.1, -429.8, 173, -429.8, 173);
    shape_.moveTo(-351.1, 260.7);
    shape_.cubicTo(-351.1, 260.7, -332, 260.7, -332, 260.7);
    shape_.cubicTo(-318.4, 260.7, -312.9, 269.1, -312.9, 276.8);
    shape_.cubicTo(-312.9, 284.6, -318.4, 293, -332, 293);
    shape_.cubicTo(-332, 293, -351.1, 293, -351.1, 293);
    shape_.cubicTo(-351.1, 293, -351.1, 225, -351.1, 225);
    shape_.cubicTo(-351.1, 225, -330, 225, -330, 225);
    shape_.cubicTo(-314.9, 225, -308.9, 234.3, -308.9, 242.8);
    shape_.cubicTo(-308.9, 251.4, -314.9, 260.7, -330, 260.7);
    shape_.cubicTo(-330, 260.7, -332, 260.7, -332, 260.7);
    shape_.moveTo(130.6, -246.2);
    shape_.cubicTo(134.8, -252.6, 141.7, -255.7, 149.9, -255.7);
    shape_.cubicTo(162.4, -255.7, 169.4, -246.7, 169.4, -235.5);
    shape_.cubicTo(169.4, -224.3, 163.8, -215.2, 150.3, -215.2);
    shape_.cubicTo(143.4, -215.2, 139.1, -216.8, 133.6, -221);
    shape_.cubicTo(133.6, -221, 136.4, -187, 136.4, -187);
    shape_.cubicTo(136.4, -187, 166, -187, 166, -187);
    shape_.moveTo(510, -186.3);
    shape_.cubicTo(500.3, -186.3, 493.9, -193.4, 493.9, -202.2);
    shape_.cubicTo(493.9, -203.6, 494.1, -205, 494.4, -206.4);
    shape_.cubicTo(495, -208.8, 496.2, -211, 497.7, -212.8);
    shape_.cubicTo(500.6, -216, 504.8, -218, 510, -218);
    shape_.cubicTo(519.7, -218, 526.1, -210.9, 526.1, -202.2);
    shape_.cubicTo(526.1, -193.4, 519.7, -186.3, 510, -186.3);
    shape_.moveTo(510, -218);
    shape_.cubicTo(498.5, -218, 490.9, -226.5, 490.9, -236.9);
    shape_.cubicTo(490.9, -238.6, 491.1, -240.3, 491.5, -241.9);
    shape_.cubicTo(492.2, -244.8, 493.6, -247.3, 495.4, -249.5);
    shape_.cubicTo(498.8, -253.3, 503.8, -255.7, 510, -255.7);
    shape_.cubicTo(521.5, -255.7, 529.1, -247.3, 529.1, -236.9);
    shape_.cubicTo(529.1, -226.5, 521.5, -218, 510, -218);
    shape_.moveTo(613.3, -249.5);
    shape_.cubicTo(617.3, -253.7, 622.1, -255.7, 628.6, -255.7);
    shape_.cubicTo(638.4, -255.7, 647.2, -248.4, 649.1, -231.3);
    shape_.cubicTo(650.4, -219.7, 651, -202.2, 643.8, -193);
    shape_.cubicTo(640.4, -188.9, 635.4, -186.3, 629.2, -186.3);
    shape_.cubicTo(617.7, -186.3, 610.1, -195.3, 610.1, -206.5);
    shape_.moveTo(610.1, -206.5);
    shape_.cubicTo(610.1, -217.7, 617.7, -226.8, 629.2, -226.8);
    shape_.cubicTo(640.7, -226.8, 648.3, -217.7, 648.3, -206.5);
    shape_.cubicTo(648.3, -204.7, 648.1, -202.8, 647.7, -201.1);
    shape_.cubicTo(646.9, -198.1, 645.6, -195.3, 643.8, -193);
    shape_.moveTo(286.7, -192.5);
    shape_.cubicTo(282.7, -188.3, 277.9, -186.3, 271.4, -186.3);
    shape_.cubicTo(261.6, -186.3, 252.8, -193.6, 250.9, -210.7);
    shape_.cubicTo(249.6, -222.3, 249, -239.8, 256.2, -249);
    shape_.cubicTo(259.6, -253.1, 264.6, -255.7, 270.8, -255.7);
    shape_.cubicTo(282.3, -255.7, 289.9, -246.7, 289.9, -235.5);
    shape_.moveTo(289.9, -235.5);
    shape_.cubicTo(289.9, -224.3, 282.3, -215.2, 270.8, -215.2);
    shape_.cubicTo(259.3, -215.2, 251.7, -224.3, 251.7, -235.5);
    shape_.cubicTo(251.7, -237.3, 251.9, -239.2, 252.3, -240.9);
    shape_.cubicTo(253.1, -243.9, 254.4, -246.7, 256.2, -249);
    shape_.moveTo(-109, -246.2);
    shape_.cubicTo(-104.8, -252.6, -98.5, -255.7, -90.2, -255.7);
    shape_.cubicTo(-80.1, -255.7, -71, -249.2, -71, -237.6);
    shape_.cubicTo(-71, -225.6, -79.7, -218.6, -92.9, -218.6);
    shape_.cubicTo(-92.9, -218.6, -93.2, -218.6, -93.2, -218.6);
    shape_.cubicTo(-79.9, -218.6, -72.7, -212.1, -72.7, -201.9);
    shape_.cubicTo(-72.7, -191.9, -80.3, -186.3, -90.4, -186.3);
    shape_.cubicTo(-97.1, -186.3, -102.5, -188.8, -106.6, -193);
    shape_.moveTo(-229, -196.3);
    shape_.cubicTo(-224.7, -189.9, -218.4, -186.3, -210.2, -186.3);
    shape_.cubicTo(-204.2, -186.3, -192.4, -189.7, -192.4, -202.8);
    shape_.cubicTo(-192.4, -222.9, -229.6, -234.3, -229.6, -255);
    shape_.cubicTo(-229.6, -255, -190.4, -255, -190.4, -255);
    shape_.moveTo(-450, -186.3);
    shape_.cubicTo(-464.3, -186.3, -471.9, -201.8, -471.9, -221);
    shape_.cubicTo(-471.9, -240.2, -464.3, -255.7, -450, -255.7);
    shape_.cubicTo(-435.7, -255.7, -428.1, -240.2, -428.1, -221);
    shape_.cubicTo(-428.1, -201.8, -435.7, -186.3, -450, -186.3);
    shape_.moveTo(38.9, -255);
    shape_.cubicTo(38.9, -255, 38.9, -187, 38.9, -187);
    shape_.moveTo(38.9, -187);
    shape_.cubicTo(38.9, -187, 8.1, -237.3, 8.1, -237.3);
    shape_.cubicTo(8.1, -237.3, 51.9, -237.3, 51.9, -237.3);
    shape_.moveTo(369.9, -187);
    shape_.cubicTo(369.9, -187, 410.1, -187, 410.1, -187);
    shape_.cubicTo(410.1, -187, 376.2, -255, 376.2, -255);
    shape_.moveTo(-338.6, -192.7);
    shape_.cubicTo(-338.6, -192.7, -328.1, -187, -328.1, -187);
    shape_.cubicTo(-328.1, -187, -328.1, -255, -328.1, -255);
    shape_.moveTo(296.9, 105);
    shape_.cubicTo(296.9, 105, 276.3, 122.2, 276.3, 122.2);
    shape_.moveTo(270, 173.7);
    shape_.cubicTo(252.5, 173.7, 243.1, 158.2, 243.1, 139);
    shape_.cubicTo(243.1, 119.8, 252.5, 104.3, 270, 104.3);
    shape_.cubicTo(287.5, 104.3, 296.9, 119.8, 296.9, 139);
    shape_.cubicTo(296.9, 158.2, 287.5, 173.7, 270, 173.7);
    shape_.moveTo(411.5, 105);
    shape_.cubicTo(411.5, 105, 401, 130.8, 401, 130.8);
    shape_.cubicTo(398.7, 135.7, 395.8, 139, 387.3, 139);
    shape_.moveTo(368.5, 105);
    shape_.cubicTo(368.5, 105, 368.5, 173, 368.5, 173);
    shape_.cubicTo(368.5, 173, 389.5, 173, 389.5, 173);
    shape_.cubicTo(404.6, 173, 410.6, 164.2, 410.6, 156);
    shape_.moveTo(410.6, 156);
    shape_.cubicTo(410.6, 147.8, 404.6, 139, 389.5, 139);
    shape_.cubicTo(389.5, 139, 368.5, 139, 368.5, 139);
    shape_.moveTo(128.9, 105);
    shape_.cubicTo(128.9, 105, 128.9, 173, 128.9, 173);
    shape_.cubicTo(128.9, 173, 150, 173, 150, 173);
    shape_.cubicTo(165.1, 173, 171.1, 163.7, 171.1, 155.1);
    shape_.moveTo(171.1, 155.1);
    shape_.cubicTo(171.1, 146.6, 165.1, 137.3, 150, 137.3);
    shape_.cubicTo(150, 137.3, 128.9, 137.3, 128.9, 137.3);
    shape_.moveTo(529, 165.3);
    shape_.cubicTo(524.4, 170.8, 517.7, 173.8, 509.4, 173.8);
    shape_.cubicTo(498.9, 173.8, 491, 167.8, 491, 158.4);
    shape_.cubicTo(491, 148.7, 498.4, 144.3, 509.4, 140.1);
    shape_.cubicTo(519.5, 136.3, 530.8, 133.4, 530.8, 120.6);
    shape_.cubicTo(530.8, 112.2, 524.9, 104.3, 510.5, 104.3);
    shape_.cubicTo(502.1, 104.3, 493.4, 108.5, 489.2, 115.2);
    shape_.moveTo(30, 173.7);
    shape_.cubicTo(12.5, 173.7, 3.1, 158.2, 3.1, 139);
    shape_.cubicTo(3.1, 119.8, 12.5, 104.3, 30, 104.3);
    shape_.cubicTo(47.5, 104.3, 56.9, 119.8, 56.9, 139);
    shape_.cubicTo(56.9, 158.2, 47.5, 173.7, 30, 173.7);
    shape_.moveTo(-67.2, 173);
    shape_.cubicTo(-67.2, 173, -67.2, 105, -67.2, 105);
    shape_.cubicTo(-67.2, 105, -112.8, 173, -112.8, 173);
    shape_.cubicTo(-112.8, 173, -112.8, 105, -112.8, 105);
    shape_.moveTo(-183.5, 105);
    shape_.cubicTo(-183.5, 105, -183.5, 173, -183.5, 173);
    shape_.cubicTo(-183.5, 173, -210, 120.2, -210, 120.2);
    shape_.cubicTo(-210, 120.2, -236.5, 173, -236.5, 173);
    shape_.cubicTo(-236.5, 173, -236.5, 105, -236.5, 105);
    shape_.moveTo(651.2, 293);
    shape_.cubicTo(651.2, 293, 651.2, 251.3, 651.2, 251.3);
    shape_.cubicTo(651.2, 232, 639.3, 224.3, 628.3, 224.3);
    shape_.cubicTo(620.9, 224.3, 613.1, 227.7, 608.8, 235.9);
    shape_.moveTo(291.1, 285.7);
    shape_.cubicTo(286.2, 290.6, 279.6, 293.7, 271.5, 293.7);
    shape_.cubicTo(254, 293.7, 243.7, 278.2, 243.7, 259);
    shape_.cubicTo(243.7, 239.8, 253.7, 224.3, 271.2, 224.3);
    shape_.cubicTo(288.6, 224.3, 296.7, 237.1, 296.3, 256.7);
    shape_.cubicTo(296.3, 256.7, 277.5, 256.7, 277.5, 256.7);
    shape_.moveTo(-98.9, 293.7);
    shape_.cubicTo(-98.9, 293.7, -113.1, 293.7, -113.1, 293.7);
    shape_.cubicTo(-113.1, 293.7, -113.1, 224.3, -113.1, 224.3);
    shape_.cubicTo(-113.1, 224.3, -98.9, 224.3, -98.9, 224.3);
    shape_.cubicTo(-73.6, 224.3, -66.9, 239.8, -66.9, 259);
    shape_.cubicTo(-66.9, 278.2, -73.6, 293.7, -98.9, 293.7);
    shape_.moveTo(367.2, 259);
    shape_.cubicTo(367.2, 259, 412.8, 259, 412.8, 259);
    shape_.moveTo(412.8, 225);
    shape_.cubicTo(412.8, 225, 412.8, 293, 412.8, 293);
    shape_.moveTo(367.2, 293);
    shape_.cubicTo(367.2, 293, 367.2, 225, 367.2, 225);
    shape_.moveTo(510, 293);
    shape_.cubicTo(510, 293, 510, 225, 510, 225);
    shape_.moveTo(49.5, 293);
    shape_.cubicTo(49.5, 293, 10.5, 293, 10.5, 293);
    shape_.cubicTo(10.5, 293, 10.5, 225, 10.5, 225);
    shape_.cubicTo(10.5, 225, 49.5, 225, 49.5, 225);
    shape_.moveTo(10.5, 259);
    shape_.cubicTo(10.5, 259, 43.4, 259, 43.4, 259);
    shape_.moveTo(169.5, 293);
    shape_.cubicTo(169.5, 293, 130.5, 293, 130.5, 293);
    shape_.cubicTo(130.5, 293, 130.5, 225, 130.5, 225);
    shape_.moveTo(130.5, 259);
    shape_.cubicTo(130.5, 259, 163.4, 259, 163.4, 259);
    shape_.moveTo(-468.5, 245.4);
    shape_.cubicTo(-468.5, 245.4, -431.5, 245.4, -431.5, 245.4);
    shape_.moveTo(-476.5, 225);
    shape_.cubicTo(-476.5, 225, -450, 293, -450, 293);
    shape_.cubicTo(-450, 293, -423.5, 225, -423.5, 225);
    shape_.moveTo(-427.1, 53);
    shape_.cubicTo(-427.1, 53, -427.1, 11.3, -427.1, 11.3);
    shape_.cubicTo(-427.1, -8, -439, -15.7, -450, -15.7);
    shape_.cubicTo(-461, -15.7, -472.9, -8, -472.9, 11.3);
    shape_.cubicTo(-472.9, 11.3, -472.9, 53, -472.9, 53);
    shape_.moveTo(129.4, 53);
    shape_.cubicTo(129.4, 53, 170.6, 53, 170.6, 53);
    shape_.cubicTo(170.6, 53, 128.1, -15, 128.1, -15);
    shape_.cubicTo(128.1, -15, 171.9, -15, 171.9, -15);
    shape_.moveTo(-177.3, 53);
    shape_.cubicTo(-177.3, 53, -192.8, -15, -192.8, -15);
    shape_.cubicTo(-192.8, -15, -210, 43.2, -210, 43.2);
    shape_.cubicTo(-210, 43.2, -227.2, -15, -227.2, -15);
    shape_.cubicTo(-227.2, -15, -242.7, 53, -242.7, 53);
    shape_.moveTo(30, 17.6);
    shape_.cubicTo(30, 17.6, 30, -15, 30, -15);
    shape_.moveTo(5.3, 53);
    shape_.cubicTo(5.3, 53, 30, 17.6, 30, 17.6);
    shape_.cubicTo(30, 17.6, 54.7, 53, 54.7, 53);
    shape_.moveTo(-67.1, 53);
    shape_.cubicTo(-67.1, 53, -112.9, -15, -112.9, -15);
    shape_.moveTo(-112.9, 53);
    shape_.cubicTo(-112.9, 53, -67.1, -15, -67.1, -15);

    for (auto& command : shape_) {
      command.point().y *= -1.0;
      command.control1().y *= -1.0;
      command.control2().y *= -1.0;
    }

    needs_stroking_ = true;
  }

  void pre() override {
    context_.begin(width(), height(), scale());
//    glClearColor(1, 1, 1, 1);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  }

  void post() override {
    context_.end();
  }

  void update() override {
    if (needs_stroking_) {
      if (width_) {
        Stroker stroker;
        stroker.set_width(width_);
        stroker.set_cap(Stroker::Cap::ROUND);
        stroker.set_join(Stroker::Join::ROUND);
        stroker.set_tolerance(10.0);
        stroke_ = stroker(shape_);
//        stroke_ = simplify(stroke_);
        stroke_.convertConicsToQuadratics();
        needs_stroking_ = false;
      } else {
        stroke_.reset();
      }
    }
  }

  void draw() override {
    takram::Shape2d shape = shape_;
    takram::Shape2d stroke = stroke_;

    for (auto& command : shape) {
      command.point() *= scale_;
      command.control1() *= scale_;
      command.control2() *= scale_;
    }
    for (auto& command : stroke) {
      command.point() *= scale_;
      command.control1() *= scale_;
      command.control2() *= scale_;
    }

    takram::nvg::Scope save;
    takram::nvg::translate(translation_);

    if (!stroke.empty()) {
      if (draws_outline_) {
        takram::nvg::beginPath();
        drawShape(stroke);
        takram::nvg::fillColor(takram::Color4d(0.0, 0.1));
        takram::nvg::fill();
        for (const auto& contour : stroke.paths()) {
          takram::nvg::beginPath();
          for (const auto& command : contour) {
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
                takram::nvg::cubicTo(command.control1(), command.control2(), command.point());
                break;
              case takram::graphics::CommandType::CLOSE:
                takram::nvg::closePath();
                break;
            }
          }
          if (contour.direction() == takram::PathDirection::CLOCKWISE) {
            takram::nvg::strokeWidth(2.0);
          } else {
            takram::nvg::strokeWidth(1.0);
          }
          takram::nvg::strokeColor(takram::Color4d(0.5));
          takram::nvg::stroke();
        }
        drawControl(stroke);
      } else {
        takram::nvg::beginPath();
        drawShape(stroke);
        takram::nvg::fillColor(takram::Color4d(0.0));
        takram::nvg::fill();
      }
    }

    if (draws_control_) {
      takram::nvg::beginPath();
      drawShape(shape);
      takram::nvg::strokeWidth(1.0);
      takram::nvg::strokeColor(takram::Color4d(0.5));
      takram::nvg::stroke();
      drawControl(shape);
    }
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
            takram::nvg::cubicTo(command.control1(), command.control2(), command.point());
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

    // Control lines
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

    // Control points
    takram::nvg::fillColor(takram::Color4d(0.5));
    takram::nvg::strokeColor(takram::Color4d(0.5));
    for (const auto& command : shape) {
      switch (command.type()) {
        case takram::graphics::CommandType::MOVE:
          takram::nvg::beginPath();
          takram::nvg::circle(command.point(), 3.0);
          takram::nvg::fill();
          break;
        case takram::graphics::CommandType::LINE:
          takram::nvg::beginPath();
          takram::nvg::circle(command.point(), 3.0);
          takram::nvg::fill();
          break;
        case takram::graphics::CommandType::QUADRATIC:
          takram::nvg::beginPath();
          takram::nvg::circle(command.control(), 3.0);
          takram::nvg::stroke();
          takram::nvg::beginPath();
          takram::nvg::circle(command.point(), 3.0);
          takram::nvg::fill();
          break;
        case takram::graphics::CommandType::CUBIC:
          takram::nvg::beginPath();
          takram::nvg::circle(command.control1(), 3.0);
          takram::nvg::stroke();
          takram::nvg::beginPath();
          takram::nvg::circle(command.control2(), 3.0);
          takram::nvg::stroke();
          takram::nvg::beginPath();
          takram::nvg::circle(command.point(), 3.0);
          takram::nvg::fill();
          break;
        default:
          break;
      }
    }
  }

  void mouseDragged() override {
    translation_ += mouse() - pmouse();
  }

  void mousePressed(const solas::MouseEvent& event) override {
    if (event.button() == solas::MouseButton::RIGHT) {
      if (draws_control_) {
        draws_control_ = false;
        draws_outline_ = true;
      } else if (draws_outline_) {
        draws_control_ = false;
        draws_outline_ = false;
      } else {
        draws_control_ = true;
        draws_outline_ = false;
      }
    }
  }

  void mouseWheel(const solas::MouseEvent& event) override {
    if (event.modifiers() % solas::KeyModifier::ALTERNATE) {
      width_ = std::max(0.0, width_ + event.wheel().y / 2.0);
      needs_stroking_ = true;
    } else {
      const auto scale = scale_;
      scale_ = std::max(1.0, scale_ * (event.wheel().y < 0.0 ? 0.8 : 1.25));
      translation_ += ((mouse() - translation_) * scale - (mouse() - translation_) * scale_) / scale;
    }
  }

 private:
  takram::nvg::Context context_;
  takram::Vec2d translation_;
  double width_;
  double scale_;
  takram::Shape2d shape_;
  takram::Shape2d stroke_;
  bool needs_stroking_;
  bool draws_control_;
  bool draws_outline_;
};

}  // namespace token

int main(int argc, char **argv) {
  solas::RunOptions options;
  options.runner().set_backend(solas::Backend::OPENGL3 |
                               solas::Backend::OPENGLES3);
  return solas::run<token::App>(argc, argv, options);
}
