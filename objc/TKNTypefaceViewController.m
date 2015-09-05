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

#import "TKNControlView.h"
#import "TKNTypefaceSampleView.h"

static char TKNTypefaceViewControllerKVOContext;

@interface TKNTypefaceViewController ()

@property (nonatomic, strong) IBOutlet NSScrollView *scrollView;
@property (nonatomic, strong) IBOutlet TKNControlView *controlView;

#pragma mark Zooming

@property (nonatomic, strong) NSArray *magnifications;
@property (nonatomic, strong) NSMutableArray *magnificationQueue;

- (void)animateMagnificationInQueue;

@end

@implementation TKNTypefaceViewController

- (instancetype)init {
  return [self initWithNibName:@"TKNTypefaceViewController"
                        bundle:[NSBundle mainBundle]];
}

- (instancetype)initWithNibName:(NSString *)nibNameOrNil
                         bundle:(NSBundle *)nibBundleOrNil {
  self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
  if (self) {
    _magnification = 1.0;
    _magnifications = @[@0.05, @0.10, @0.15, @0.20, @0.30, @0.40, @0.50, @0.75,
                      @1.00, @1.50, @2.00, @3.00, @4.00, @6.00, @8.00,
                      @10.00, @15.00, @20.00, @30.00];
    _magnificationQueue = [NSMutableArray array];
  }
  return self;
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
  _sampleView = [[TKNTypefaceSampleView alloc] initWithFrame:_scrollView.frame];
  _sampleView.typeface = _typeface;
  _scrollView.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
  _scrollView.hasHorizontalScroller = YES;
  _scrollView.hasVerticalScroller = YES;
  _scrollView.documentView = _sampleView;
  _scrollView.allowsMagnification = YES;
  _scrollView.minMagnification = [_magnifications.firstObject doubleValue];
  _scrollView.maxMagnification = [_magnifications.lastObject doubleValue];

  // Inject self to the responder chain
  self.nextResponder = _scrollView.contentView.nextResponder;
   _scrollView.contentView.nextResponder = self;
}

- (void)scrollWheel:(NSEvent *)event {
  if (event.modifierFlags & NSAlternateKeyMask) {
    self.magnification *= event.deltaY < 0.0 ? 0.9 : 1.0 / 0.9;
  } else {
    [_scrollView scrollWheel:event];
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

#pragma mark Zooming

- (void)setMagnification:(double)magnification {
  [self setMagnification:magnification animated:NO];
}

- (void)setMagnification:(double)magnification animated:(BOOL)animated {
  magnification = MIN(MAX(
      magnification,
      _scrollView.minMagnification),
      _scrollView.maxMagnification);
  if (magnification == _magnification) {
    return;
  }
  [self willChangeValueForKey:@"magnification"];
  _magnification = magnification;
  [self didChangeValueForKey:@"magnification"];
  if (animated) {
    [_magnificationQueue addObject:[NSNumber numberWithDouble:magnification]];
    if (_magnificationQueue.count == 1) {
      [self animateMagnificationInQueue];
    }
    return;
  } else {
    _scrollView.magnification = magnification;
  }
}

- (void)animateMagnificationInQueue {
  __weak typeof(self) wself = self;
  [NSAnimationContext runAnimationGroup:^(NSAnimationContext *context) {
    _scrollView.animator.magnification =
        [_magnificationQueue.firstObject doubleValue];
    [_magnificationQueue removeObjectAtIndex:0];
  } completionHandler:^{
    if (_magnificationQueue.count) {
      [wself animateMagnificationInQueue];
    }
  }];
}

- (IBAction)zoomIn:(id)sender {
  double magnification = _magnification;
  double result = magnification;
  NSEnumerator *enumerator = _magnifications.objectEnumerator;
  for (NSNumber *number; number = [enumerator nextObject];) {
    double proposed = number.doubleValue;
    if (magnification >= proposed) {
      continue;
    }
    if (result <= magnification && magnification < proposed) {
      result = proposed;
      break;
    }
    result = proposed;
  }
  [self setMagnification:result animated:YES];
}

- (IBAction)zoomOut:(id)sender {
  double magnification = _magnification;
  double result = magnification;
  NSEnumerator *enumerator = _magnifications.reverseObjectEnumerator;
  for (NSNumber *number; number = [enumerator nextObject];) {
    double proposed = number.doubleValue;
    if (magnification <= proposed) {
      continue;
    }
    if (result >= magnification && magnification > proposed) {
      result = proposed;
      break;
    }
    result = proposed;
  }
  [self setMagnification:result animated:YES];
}

@end
