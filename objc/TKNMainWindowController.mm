//
//  TKNMainWindowController.mm
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

#import "TKNMainWindowController.h"

#include <memory>
#include <utility>

#include "solas.h"
#include "token/view.h"

@interface TKNMainWindowController () {
 @private
  token::View *_view;
}

@property (nonatomic, strong) IBOutlet NSView *contentView;
@property (nonatomic, strong) IBOutlet NSView *controlView;

@end

@implementation TKNMainWindowController

- (instancetype)init {
  NSString *path = [[NSBundle mainBundle]
      pathForResource:@"TKNMainWindowController"
               ofType:@"nib"];
  self = [super initWithWindowNibPath:path owner:self];
  if (self) {
    solas::RunnerOptions options;
    options.set_backend(solas::Backend::OPENGL3);
    auto view = std::make_unique<token::View>();
    _view = view.get();
    auto runner = std::make_unique<solas::Runner>(std::move(view), options);
    _runner = [[SLSRunner alloc] initWithRunnable:std::move(runner)];
    _viewController = [[SLSNSViewController alloc] initWithRunner:_runner];
  }
  return self;
}

- (void)windowDidLoad {
  [super windowDidLoad];
  self.window.movableByWindowBackground = YES;
  self.window.titleVisibility = NSWindowTitleHidden;
  self.window.titlebarAppearsTransparent = YES;
  self.window.styleMask |= NSFullSizeContentViewWindowMask;
  NSView *view = _viewController.view;
  view.translatesAutoresizingMaskIntoConstraints = NO;
  [self.contentView addSubview:view positioned:NSWindowBelow relativeTo:nil];
  [self.contentView addConstraints:[NSLayoutConstraint
      constraintsWithVisualFormat:@"|-0-[view]-0-|"
      options:0
      metrics:nil
      views:NSDictionaryOfVariableBindings(view)]];
  [self.contentView addConstraints:[NSLayoutConstraint
      constraintsWithVisualFormat:@"V:|-0-[view]-0-|"
      options:0
      metrics:nil
      views:NSDictionaryOfVariableBindings(view)]];
}

- (void)setPhysicalAscent:(double)physicalAscent {
  if (_physicalAscent != physicalAscent) {
    _physicalAscent = physicalAscent;
  }
}

- (void)setPhysicalStrokeWidth:(double)physicalStrokeWidth {
  if (_physicalStrokeWidth != physicalStrokeWidth) {
    _physicalStrokeWidth = physicalStrokeWidth;
  }
}

- (void)generateFont {
}

@end
