//
//  TKNNumericTextField.m
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

#import "TKNNumericTextField.h"

#import <CoreImage/CoreImage.h>

@interface TKNNumericTextField ()

@property (nonatomic, assign) double initialValue;
@property (nonatomic, assign) CGPoint initialLocation;
@property (nonatomic, strong, nonnull) NSTrackingArea *trackingArea;

- (void)setUpNumericTextField;
- (void)setUpTrackingArea;

@end

@implementation TKNNumericTextField

- (instancetype)initWithFrame:(CGRect)frame {
  self = [super initWithFrame:frame];
  if (self) {
    [self setUpNumericTextField];
  }
  return self;
}

- (instancetype)initWithCoder:(NSCoder *)coder {
  self = [super initWithCoder:coder];
  if (self) {
    [self setUpNumericTextField];
  }
  return self;
}

- (void)setUpNumericTextField {
  [self setUpTrackingArea];
  self.editable = NO;
  self.selectable = NO;
  NSNotificationCenter *defaultCenter = [NSNotificationCenter defaultCenter];
  [defaultCenter addObserver:self
                    selector:@selector(controlTextDidEndEditing:)
                        name:NSControlTextDidEndEditingNotification
                      object:self];
}

- (void)setUpTrackingArea {
  if (_trackingArea) {
    [self removeTrackingArea:_trackingArea];
  }
  self.trackingArea = [[NSTrackingArea alloc]
      initWithRect:self.bounds
           options:(NSTrackingMouseEnteredAndExited |
                    NSTrackingMouseMoved |
                    NSTrackingActiveInKeyWindow)
             owner:self
          userInfo:nil];
  [self addTrackingArea:_trackingArea];
}

- (void)dealloc {
  NSNotificationCenter *defaultCenter = [NSNotificationCenter defaultCenter];
  [defaultCenter removeObserver:self
                           name:NSControlTextDidEndEditingNotification
                         object:self];
}

- (BOOL)mouseDownCanMoveWindow {
  return NO;
}

- (void)setFrameSize:(CGSize)size {
  [super setFrameSize:size];
  [self setUpTrackingArea];
}

- (void)mouseDown:(NSEvent *)event {
  [super mouseDown:event];
  self.initialLocation = event.locationInWindow;
  self.initialValue = self.doubleValue;
}

- (void)mouseDragged:(NSEvent *)event {
  [super mouseDragged:event];
  CGPoint location = event.locationInWindow;
  CGFloat delta = round((location.x - _initialLocation.x) / 2.0) * _step;
  CGFloat result = _initialValue + delta;

  // Propagate the change through binding, and store the value after
  // propagation back to the double value of the text field.
  NSDictionary *bindingInfo = [self infoForBinding:NSValueBinding];
  id observedObject = bindingInfo[NSObservedObjectKey];
  NSString *keyPath = bindingInfo[NSObservedKeyPathKey];
  [observedObject setValue:[NSNumber numberWithDouble:result]
                forKeyPath:keyPath];
  self.doubleValue = [[observedObject valueForKeyPath:keyPath] doubleValue];
}

- (void)mouseUp:(NSEvent *)event {
  [super mouseUp:event];
  if (!event.clickCount) {
    return;
  }
  CGPoint location = [self convertPoint:event.locationInWindow
                               fromView:self.window.contentView];
  if (CGRectContainsPoint(self.bounds, location)) {
    self.editable = YES;
    self.selectable = YES;
    [self selectText:self];
    [self resetCursorRects];
  }
}

- (void)resetCursorRects {
  if (!self.editable) {
    [self addCursorRect:self.bounds cursor:[NSCursor resizeLeftRightCursor]];
  } else {
    [super resetCursorRects];
  }
}

- (void)controlTextDidEndEditing:(NSNotification *)notification {
  NSDictionary *bindingInfo = [self infoForBinding:NSValueBinding];
  id observedObject = bindingInfo[NSObservedObjectKey];
  NSString *keyPath = bindingInfo[NSObservedKeyPathKey];
  self.doubleValue = [[observedObject valueForKeyPath:keyPath] doubleValue];
  self.editable = NO;
  self.selectable = NO;
  [self resetCursorRects];
}

@end
