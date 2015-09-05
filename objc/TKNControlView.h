//
//  TKNControlView.h
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

#import <AppKit/AppKit.h>

@interface TKNControlView : NSView {
  // Keep a reference to the filters for later modification
  CIFilter *_blurFilter, *_saturationFilter;
  CALayer *_hostedLayer;
}

/** The layer will be tinted using the tint color. By default it is a 70% White Color */
@property (strong,nonatomic) NSColor *tintColor;

/** To get more vibrant colors, a filter to increase the saturation of the colors can be applied. The default value is 2.5. */
@property (assign,nonatomic) float saturationFactor;

/** The blur radius defines the strength of the Gaussian Blur filter. The default value is 20.0. */
@property (assign,nonatomic) float blurRadius;

/** Use the following property names in the  User Defined Runtime Attributes in Interface Builder to set up your RMBlurredView on the fly. Note: The color can be set up, too, using 'tintColor'. */
@property (strong,nonatomic) NSNumber *saturationFactorNumber;
@property (strong,nonatomic) NSNumber *blurRadiusNumber;

@end
