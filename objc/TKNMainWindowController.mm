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

#import "TKNSettingsView.h"
#import "TKNSettingsViewController.h"
#import "TKNTypeViewController.h"

@interface TKNMainWindowController ()

@property (nonatomic, strong) IBOutlet NSView *controlView;
@property (nonatomic, strong) NSView *typeView;
@property (nonatomic, strong) TKNSettingsView *settingsView;
@property (nonatomic, strong) NSMutableArray *collapsedSettngsViewConstraints;
@property (nonatomic, strong) NSMutableArray *expandedSettngsViewConstraints;
@property (nonatomic, assign) CGRect windowFrame;

@end

@implementation TKNMainWindowController

- (instancetype)init {
  NSString *path = [[NSBundle mainBundle]
      pathForResource:@"TKNMainWindowController"
               ofType:@"nib"];
  self = [super initWithWindowNibPath:path owner:self];
  if (self) {
    _typeViewController = [[TKNTypeViewController alloc] init];
    _settingsViewController = [[TKNSettingsViewController alloc] init];
    _collapsedSettngsViewConstraints = [NSMutableArray array];
    _expandedSettngsViewConstraints = [NSMutableArray array];
  }
  return self;
}

- (void)dealloc {
  NSNotificationCenter *defaultCenter = [NSNotificationCenter defaultCenter];
  [defaultCenter removeObserver:self
                           name:NSWindowDidResizeNotification
                         object:self.window];
}

- (void)windowDidLoad {
  [super windowDidLoad];
  self.window.movableByWindowBackground = YES;
  self.window.titleVisibility = NSWindowTitleHidden;
  self.window.titlebarAppearsTransparent = YES;
  self.window.styleMask |= NSFullSizeContentViewWindowMask;
  NSNotificationCenter *defaultCenter = [NSNotificationCenter defaultCenter];
  [defaultCenter addObserver:self
                    selector:@selector(windowDidResize:)
                        name:NSWindowDidResizeNotification
                      object:self.window];
  NSView *view = self.window.contentView;

  // Appearances
  NSString *darkAppearance = NSAppearanceNameVibrantDark;
  NSString *defaultAppearance = NSAppearanceNameAqua;

  // Type
  _typeView = _typeViewController.view;
  _typeView.translatesAutoresizingMaskIntoConstraints = NO;
  _typeView.appearance = [NSAppearance appearanceNamed:defaultAppearance];
  [view addSubview:_typeView];
  [view addConstraints:[NSLayoutConstraint
      constraintsWithVisualFormat:@"|-0-[_typeView]-0-|"
      options:0
      metrics:nil
      views:NSDictionaryOfVariableBindings(_typeView)]];
  [view addConstraints:[NSLayoutConstraint
      constraintsWithVisualFormat:@"V:|-0-[_typeView]"
      options:0
      metrics:nil
      views:NSDictionaryOfVariableBindings(_typeView)]];

  // Settings
  _settingsView = (TKNSettingsView *)_settingsViewController.view;
  _settingsView.appearance = [NSAppearance appearanceNamed:darkAppearance];
  _settingsView.translatesAutoresizingMaskIntoConstraints = NO;
  [view addSubview:_settingsView];

  // Control
  _controlView.translatesAutoresizingMaskIntoConstraints = NO;
  [view addSubview:_controlView];
  [view addConstraints:[NSLayoutConstraint
      constraintsWithVisualFormat:@"V:[_controlView][_settingsView]"
      options:0
      metrics:nil
      views:NSDictionaryOfVariableBindings(_controlView, _settingsView)]];
  [view addConstraint:[NSLayoutConstraint
      constraintWithItem:_controlView
      attribute:NSLayoutAttributeLeading
      relatedBy:NSLayoutRelationEqual
      toItem:_settingsView
      attribute:NSLayoutAttributeLeading
      multiplier:1.0
      constant:0.0]];
  [view addConstraint:[NSLayoutConstraint
      constraintWithItem:_controlView
      attribute:NSLayoutAttributeTrailing
      relatedBy:NSLayoutRelationEqual
      toItem:_settingsView
      attribute:NSLayoutAttributeTrailing
      multiplier:1.0
      constant:0.0]];

  // Layout constraints
  [_collapsedSettngsViewConstraints addObjectsFromArray:[NSLayoutConstraint
      constraintsWithVisualFormat:@"|-0-[_settingsView]-0-|"
      options:0
      metrics:nil
      views:NSDictionaryOfVariableBindings(_settingsView)]];
  [_collapsedSettngsViewConstraints addObjectsFromArray:[NSLayoutConstraint
      constraintsWithVisualFormat:@"V:[_settingsView]-0-|"
      options:0
      metrics:nil
      views:NSDictionaryOfVariableBindings(_settingsView)]];
  [_collapsedSettngsViewConstraints addObjectsFromArray:[NSLayoutConstraint
      constraintsWithVisualFormat:@"V:[_typeView]-0-[_settingsView]"
      options:0
      metrics:nil
      views:NSDictionaryOfVariableBindings(_typeView, _settingsView)]];
  [_expandedSettngsViewConstraints addObjectsFromArray:[NSLayoutConstraint
      constraintsWithVisualFormat:@"[_settingsView(900)]-0-|"
      options:0
      metrics:nil
      views:NSDictionaryOfVariableBindings(_settingsView)]];
  [_expandedSettngsViewConstraints addObjectsFromArray:[NSLayoutConstraint
      constraintsWithVisualFormat:@"V:[_settingsView]-0-|"
      options:0
      metrics:nil
      views:NSDictionaryOfVariableBindings(_settingsView)]];
  [_expandedSettngsViewConstraints addObjectsFromArray:[NSLayoutConstraint
      constraintsWithVisualFormat:@"V:[_typeView]-0-|"
      options:0
      metrics:nil
      views:NSDictionaryOfVariableBindings(_typeView)]];
  [self windowDidResize:nil];
}

- (void)windowDidResize:(NSNotification *)notification {
  CGFloat threshold = 900.0;
  NSView *view = self.window.contentView;
  if (self.window.frame.size.width < threshold) {
    if (!_windowFrame.size.width || _windowFrame.size.width >= threshold) {
      [view removeConstraints:_expandedSettngsViewConstraints];
      [view addConstraints:_collapsedSettngsViewConstraints];
      _settingsView.rounded = NO;
    }
  } else {
    if (!_windowFrame.size.width || _windowFrame.size.width < threshold) {
      [view removeConstraints:_collapsedSettngsViewConstraints];
      [view addConstraints:_expandedSettngsViewConstraints];
      _settingsView.rounded = YES;
    }
  }
  _windowFrame = self.window.frame;
}

#pragma mark Actions

- (IBAction)exportFont:(id)sender {
}

- (IBAction)installFont:(id)sender {
}

@end
