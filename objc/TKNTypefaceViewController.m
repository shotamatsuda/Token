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

@property (nonatomic, strong) NSMutableArray *magnificationQueue;

- (void)animateMagnificationInQueue;
- (void)contentViewFrameDidChange:(NSNotification *)notification;

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
    _magnificationQueue = [NSMutableArray array];
  }
  return self;
}

- (void)dealloc {
  NSNotificationCenter *defaultCenter = [NSNotificationCenter defaultCenter];
  [defaultCenter removeObserver:self
                           name:NSViewFrameDidChangeNotification
                         object:_sampleView];
  [defaultCenter removeObserver:self
                           name:NSViewFrameDidChangeNotification
                         object:_scrollView];
  NSArray *keyPaths = @[@"capHeight", @"strokeWidth",
                        @"capHeightEqualsUnitsPerEM",
                        @"capHeightUnit", @"strokeWidthUnit"];
  for (NSString *keyPath in keyPaths) {
    [_typeface removeObserver:self
                   forKeyPath:keyPath
                      context:&TKNTypefaceViewControllerKVOContext];
  }
}

- (void)viewDidLoad {
  [super viewDidLoad];

  // Sample view
  _sampleView = [[TKNTypefaceSampleView alloc] initWithFrame:_scrollView.frame];
  _sampleView.typeface = _typeface;

  // Scroll view
  _scrollView.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
  _scrollView.hasHorizontalScroller = YES;
  _scrollView.hasVerticalScroller = YES;
  _scrollView.documentView = _sampleView;
  _scrollView.allowsMagnification = YES;
  _scrollView.minMagnification = 0.03125;
  _scrollView.maxMagnification = 64.0;

  // Observe for the frames' changes for zooming to fit
  NSNotificationCenter *defaultCenter = [NSNotificationCenter defaultCenter];
  [defaultCenter addObserver:self
                    selector:@selector(contentViewFrameDidChange:)
                        name:NSViewFrameDidChangeNotification
                      object:_sampleView];
  [defaultCenter addObserver:self
                    selector:@selector(contentViewFrameDidChange:)
                        name:NSViewFrameDidChangeNotification
                      object:_scrollView];

  // Inject self to the responder chain
  self.nextResponder = _scrollView.contentView.nextResponder;
  _scrollView.contentView.nextResponder = self;
}

- (void)scrollWheel:(NSEvent *)event {
  if (event.type == NSScrollWheel && event.modifierFlags & NSAlternateKeyMask) {
    self.shouldZoomToFit = NO;
    CGPoint center = [_scrollView.contentView
        convertPoint:event.locationInWindow
        fromView:self.view.window.contentView];
    [self willChangeValueForKey:@"magnification"];
    CGFloat scale = (event.scrollingDeltaY < 0.0 ? 0.9 : 1.0 / 0.9);
    _magnification = MIN(MAX(
        _magnification * scale,
        _scrollView.minMagnification),
        _scrollView.maxMagnification);
    [self didChangeValueForKey:@"magnification"];
    [_scrollView setMagnification:_magnification centeredAtPoint:center];
  } else {
    [_scrollView scrollWheel:event];
  }
}

- (void)magnifyWithEvent:(NSEvent *)event {
  self.shouldZoomToFit = NO;
  CGPoint center = [_scrollView.contentView
      convertPoint:event.locationInWindow
      fromView:self.view.window.contentView];
  [self willChangeValueForKey:@"magnification"];
  _magnification = MIN(MAX(
      _magnification * (1.0 + event.magnification),
      _scrollView.minMagnification),
      _scrollView.maxMagnification);
  [self didChangeValueForKey:@"magnification"];
  [_scrollView setMagnification:_magnification centeredAtPoint:center];
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
    NSArray *keyPaths = @[@"capHeight", @"strokeWidth",
                          @"capHeightEqualsUnitsPerEM",
                          @"capHeightUnit", @"strokeWidthUnit"];
    for (NSString *keyPath in keyPaths) {
      [_typeface removeObserver:self
                     forKeyPath:keyPath
                        context:&TKNTypefaceViewControllerKVOContext];
    }
    _typeface = typeface;
    for (NSString *keyPath in keyPaths) {
      [_typeface addObserver:self
                  forKeyPath:keyPath
                     options:NSKeyValueObservingOptionNew
                     context:&TKNTypefaceViewControllerKVOContext];
    }
  }
}

#pragma mark Parameters

- (void)setInverted:(BOOL)inverted {
  if (inverted != _inverted) {
    _inverted = inverted;
    _sampleView.inverted = inverted;
    if (_inverted) {
      _scrollView.backgroundColor = [NSColor blackColor];
    } else {
      _scrollView.backgroundColor = [NSColor whiteColor];
    }
  }
}

- (void)setOutlined:(BOOL)outlined {
  if (outlined != _outlined) {
    _outlined = outlined;
    _sampleView.outlined = outlined;
  }
}

- (void)toggleInverted:(id)sender {
  self.inverted = !_inverted;
}

- (void)toggleOutlined:(id)sender {
  self.outlined = !_outlined;
}

#pragma mark Zooming

- (void)setMagnification:(CGFloat)magnification {
  [self setMagnification:magnification animated:NO];
}

- (void)setMagnification:(CGFloat)magnification animated:(BOOL)animated {
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
    _sampleView.needsDisplay = YES;
    if (_magnificationQueue.count) {
      [wself animateMagnificationInQueue];
    }
  }];
}

- (void)contentViewFrameDidChange:(NSNotification *)notification {
  if (_shouldZoomToFit) {
    [self zoomToFitAnimated:NO];
  }
}

- (void)setShouldZoomToFit:(BOOL)shouldZoomToFit {
  if (shouldZoomToFit != _shouldZoomToFit) {
    _shouldZoomToFit = shouldZoomToFit;
    [self zoomToFitAnimated:YES];
  }
}

- (void)zoomToFitAnimated:(BOOL)animated {
  CGRect bounds = _scrollView.bounds;
  CGRect frame = [_scrollView.documentView frame];
  CGFloat magnification = MIN(
      bounds.size.width / frame.size.width,
      bounds.size.height / frame.size.height);
  CGPoint center = CGPointMake(CGRectGetMidX(frame), CGRectGetMidY(frame));
  center = [_scrollView.contentView convertPoint:center fromView:_scrollView];
  [self setMagnification:magnification animated:animated];
}

- (void)zoomToFit:(id)sender {
  [self zoomToFitAnimated:YES];
}

- (IBAction)zoomIn:(id)sender {
  self.shouldZoomToFit = NO;
  CGFloat magnification = _magnification;
  CGFloat result = magnification;
  CGFloat proposed = _scrollView.minMagnification;
  for (int i = 0;; ++i) {
    if (proposed >= 1.0) {
      proposed /= i % 2 ? 3.0 / 4.0 : 2.0 / 3.0;
    } else {
      proposed /= i % 2 ? 2.0 / 3.0 : 3.0 / 4.0;
    }
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
  self.shouldZoomToFit = NO;
  CGFloat magnification = _magnification;
  CGFloat result = magnification;
  CGFloat proposed = _scrollView.maxMagnification;
  for (int i = 0;; ++i) {
    if (proposed <= 1.0) {
      proposed *= i % 2 ? 3.0 / 4.0 : 2.0 / 3.0;
    } else {
      proposed *= i % 2 ? 2.0 / 3.0 : 3.0 / 4.0;
    }
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
