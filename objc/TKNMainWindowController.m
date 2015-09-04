//
//  TKNMainWindowController.m
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
#import "TKNTypefaceViewController.h"

@interface TKNMainWindowController ()

@property (nonatomic, strong) IBOutlet NSView *controlView;
@property (nonatomic, strong) NSView *typefaceView;
@property (nonatomic, strong) TKNSettingsView *settingsView;
@property (nonatomic, assign) CGRect windowFrame;

@end

@implementation TKNMainWindowController

- (instancetype)init {
  NSString *path = [[NSBundle mainBundle]
      pathForResource:@"TKNMainWindowController"
               ofType:@"nib"];
  self = [super initWithWindowNibPath:path owner:self];
  if (self) {
    _typefaceViewController = [[TKNTypefaceViewController alloc] init];
    _settingsViewController = [[TKNSettingsViewController alloc] init];
    NSBundle *bundle = [NSBundle mainBundle];
    NSString *Path = [bundle pathForResource:@"Token" ofType:@"ufo"];
    _typeface = [[TKNTypeface alloc] initWithFileAtPath:Path];
    _typefaceViewController.typeface = _typeface;
    _settingsViewController.typeface = _typeface;
  }
  return self;
}

- (void)windowDidLoad {
  [super windowDidLoad];
  self.window.movableByWindowBackground = YES;
  self.window.titleVisibility = NSWindowTitleHidden;
  self.window.titlebarAppearsTransparent = YES;
  self.window.styleMask |= NSFullSizeContentViewWindowMask;
  NSView *view = self.window.contentView;

  // Appearances
  NSString *darkAppearance = NSAppearanceNameVibrantDark;
  NSString *defaultAppearance = NSAppearanceNameAqua;

  // Typeface
  _typefaceView = _typefaceViewController.view;
  _typefaceView.translatesAutoresizingMaskIntoConstraints = NO;
  _typefaceView.appearance = [NSAppearance appearanceNamed:defaultAppearance];
  [view addSubview:_typefaceView];
  [view addConstraints:[NSLayoutConstraint
      constraintsWithVisualFormat:@"|-0-[_typefaceView]-0-|"
      options:0
      metrics:nil
      views:NSDictionaryOfVariableBindings(_typefaceView)]];
  [view addConstraints:[NSLayoutConstraint
      constraintsWithVisualFormat:@"V:|-0-[_typefaceView]"
      options:0
      metrics:nil
      views:NSDictionaryOfVariableBindings(_typefaceView)]];

  // Settings
  _settingsView = (TKNSettingsView *)_settingsViewController.view;
  _settingsView.appearance = [NSAppearance appearanceNamed:darkAppearance];
  _settingsView.translatesAutoresizingMaskIntoConstraints = NO;
  [view addSubview:_settingsView];
  [view addConstraints:[NSLayoutConstraint
      constraintsWithVisualFormat:@"|-0-[_settingsView]-0-|"
      options:0
      metrics:nil
      views:NSDictionaryOfVariableBindings(_settingsView)]];
  [view addConstraints:[NSLayoutConstraint
      constraintsWithVisualFormat:@"V:[_settingsView]-0-|"
      options:0
      metrics:nil
      views:NSDictionaryOfVariableBindings(_settingsView)]];
  [view addConstraints:[NSLayoutConstraint
      constraintsWithVisualFormat:@"V:[_typefaceView]-0-[_settingsView]"
      options:0
      metrics:nil
      views:NSDictionaryOfVariableBindings(_typefaceView, _settingsView)]];

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
}

#pragma mark Actions

- (IBAction)exportFont:(id)sender {
  NSSavePanel *panel = [NSSavePanel savePanel];
  panel.nameFieldStringValue = [_typeface.postscriptFontName
      stringByAppendingPathExtension:@"ufo"];
  [panel beginSheetModalForWindow:self.window
                completionHandler:^(NSInteger result) {
    if (result == NSFileHandlingPanelOKButton) {
      [_typeface saveToFile:panel.URL.path];
    }
  }];
}

- (IBAction)installFont:(id)sender {
}

@end
