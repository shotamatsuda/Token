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

#import "TKNFilePaths.h"
#import "TKNSettingsViewController.h"
#import "TKNTypefaceViewController.h"
#import "TKNWelcomeSheetController.h"

@interface TKNMainWindowController ()

@property (nonatomic, strong) NSView *typefaceView;
@property (nonatomic, strong) NSView *settingsView;
@property (nonatomic, assign) CGRect windowFrame;
@property (nonatomic, strong) TKNWelcomeSheetController *welcomeSheetController;

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
    NSString *path = [bundle pathForResource:@"typeface" ofType:nil];
    _typeface = [[TKNTypeface alloc] initWithFileAtPath:path];
    _typefaceViewController.typeface = _typeface;
    _typefaceViewController.shouldZoomToFit = YES;
    _settingsViewController.typeface = _typeface;
  }
  return self;
}

- (void)windowDidLoad {
  [super windowDidLoad];
  NSWindow *window = self.window;
  window.movableByWindowBackground = YES;
  window.titleVisibility = NSWindowTitleHidden;
  window.titlebarAppearsTransparent = YES;
  window.styleMask |= NSFullSizeContentViewWindowMask;

  // Appearances
  NSString *darkAppearance = NSAppearanceNameVibrantDark;
  NSString *defaultAppearance = NSAppearanceNameAqua;

  // Typeface
  NSView *view = window.contentView;
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
  _settingsView = _settingsViewController.view;
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

  // Check for FDK
  dispatch_async(dispatch_get_main_queue(), ^{
    NSFileManager *fileManager = [NSFileManager defaultManager];
    if (![fileManager fileExistsAtPath:TKNAdobeFDKPath()]) {
      [self installAdobeFDK:self];
    }
  });
}

#pragma mark Actions

- (IBAction)exportFont:(id)sender {
  NSSavePanel *panel = [NSSavePanel savePanel];
  panel.nameFieldStringValue = [_typeface.postscriptName
      stringByAppendingPathExtension:@"otf"];
  [panel beginSheetModalForWindow:self.window
                completionHandler:^(NSInteger result) {
    if (result == NSFileHandlingPanelOKButton) {
      [_typeface saveToFile:panel.URL.path];
    }
  }];
}

- (IBAction)installFont:(id)sender {
  NSString *fontName = [_typeface.postscriptName
      stringByAppendingPathExtension:@"otf"];
  NSString *installPath = [[[NSSearchPathForDirectoriesInDomains(
      NSLibraryDirectory, NSUserDomainMask, YES).firstObject
          stringByAppendingPathComponent:@"Fonts"]
              stringByAppendingPathComponent:_typeface.familyName]
                  stringByAppendingPathComponent:fontName];
  if ([_typeface saveToFile:installPath]) {
    [[NSWorkspace sharedWorkspace] openFile:
        installPath.stringByDeletingLastPathComponent];
  }
}

- (void)zoomIn:(id)sender {
  [_typefaceViewController zoomIn:sender];
}

- (void)zoomOut:(id)sender {
  [_typefaceViewController zoomOut:sender];
}

- (void)installAdobeFDK:(id)sender {
  _welcomeSheetController = [[TKNWelcomeSheetController alloc] init];
  [self.window beginSheet:_welcomeSheetController.window
        completionHandler:^(NSModalResponse returnCode) {}];
}

- (void)uninstallAdobeFDK:(id)sender {
  NSAlert *alert = [[NSAlert alloc] init];
  alert.alertStyle = NSWarningAlertStyle;
  alert.messageText = NSLocalizedString(
      @"Are you sure you want to permanently erase Adobe FDK?", @"");
  alert.informativeText = NSLocalizedString(
      @"You can’t undo this action.", @"");
  [alert addButtonWithTitle:NSLocalizedString(@"Uninstall", @"")];
  [alert addButtonWithTitle:NSLocalizedString(@"Cancel", @"")];
  void (^handler)(NSModalResponse) = ^(NSModalResponse returnCode) {
    if (returnCode != NSAlertFirstButtonReturn) {
      return;
    }
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSError *error = nil;
    NSString *applicationSupportPath = TKNPrivateApplicationSupportPath();
    if ([fileManager fileExistsAtPath:applicationSupportPath]) {
      if (![fileManager removeItemAtPath:applicationSupportPath error:&error]) {
        [[NSAlert alertWithError:error]
            beginSheetModalForWindow:NSApp.mainWindow
            completionHandler:nil];
        return;
      }
    }
    NSString *libraryPath = TKNPrivateLibraryPath();
    if ([fileManager fileExistsAtPath:libraryPath]) {
      if (![fileManager removeItemAtPath:libraryPath error:&error]) {
        [[NSAlert alertWithError:error]
            beginSheetModalForWindow:NSApp.mainWindow
            completionHandler:nil];
        return;
      }
    }
  };
  [alert beginSheetModalForWindow:self.window completionHandler:handler];
}

@end
