//
//  TKNTypefaceViewController.m
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

#import "TKNTypefaceViewController.h"

#import "TKNTypefaceSampleView.h"

static char TKNTypefaceViewControllerKVOContext;

@implementation TKNTypefaceViewController

- (instancetype)init {
  return [self initWithNibName:@"TKNTypefaceViewController"
                        bundle:[NSBundle mainBundle]];
}

- (void)dealloc {
  NSArray *keyPaths = @[@"capHeight", @"width",
                        @"capHeightEqualsUnitsPerEM",
                        @"capHeightUnit", @"widthUnit"];
  for (NSString *keyPath in keyPaths) {
    [_typeface removeObserver:self
                   forKeyPath:keyPath
                      context:&TKNTypefaceViewControllerKVOContext];
  }
}

- (void)viewDidLoad {
  [super viewDidLoad];
  _sampleView = [[TKNTypefaceSampleView alloc] initWithFrame:self.view.frame];
  _sampleView.typeface = _typeface;
  NSScrollView *scrollView = (NSScrollView *)self.view;
  scrollView.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
  scrollView.hasHorizontalScroller = YES;
  scrollView.hasVerticalScroller = YES;
  scrollView.documentView = _sampleView;
}

- (void)setTypeface:(TKNTypeface *)typeface {
  if (typeface != _typeface) {
    _typeface = typeface;
    NSArray *keyPaths = @[@"capHeight", @"width",
                          @"capHeightEqualsUnitsPerEM",
                          @"capHeightUnit", @"widthUnit"];
    for (NSString *keyPath in keyPaths) {
      [_typeface addObserver:self
                  forKeyPath:keyPath
                     options:NSKeyValueObservingOptionNew
                     context:&TKNTypefaceViewControllerKVOContext];
    }
  }
}

- (void)observeValueForKeyPath:(NSString *)keyPath
                      ofObject:(id)object
                        change:(NSDictionary *)change
                       context:(void *)context {
  if (context == &TKNTypefaceViewControllerKVOContext) {
    if (object == _typeface) {
      _sampleView.needsDisplay = YES;
    }
  } else {
    [super observeValueForKeyPath:keyPath
                         ofObject:object
                           change:change
                          context:context];
  }
}

@end
