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

#import "Token-Swift.h"
#import "TKNTypefaceViewController.h"
#import "TKNWelcomeSheetController.h"

@interface TKNMainWindowController ()

#pragma mark Sheet Controllers

@property (nonatomic, strong, nonnull)
    TKNWelcomeSheetController *welcomeSheetController;

@end

@implementation TKNMainWindowController

- (void)windowDidLoad {
  [super windowDidLoad];
  NSWindow *window = self.window;
  window.movableByWindowBackground = YES;
  window.titlebarAppearsTransparent = YES;
  window.titleVisibility = NSWindowTitleHidden;
  window.styleMask |= NSFullSizeContentViewWindowMask;

  // Prepare a typeface
  NSBundle *bundle = [NSBundle mainBundle];
  NSString *path = [bundle pathForResource:@"typeface" ofType:nil];
  self.typeface = [[TKNTypeface alloc] initWithFileAtPath:path];
  self.typefaceViewController.typeface = _typeface;
  self.settingsViewController.typeface = _typeface;

  // Check for Adobe FDK and show the welcome sheet if necessary.
  dispatch_async(dispatch_get_main_queue(), ^{
    NSFileManager *fileManager = [NSFileManager defaultManager];
    if (![fileManager fileExistsAtPath:TKNFilePath.adobeFDK]) {
      [self installAdobeFDK:self];
    }
  });
}

#pragma mark View Controllers

- (TKNMainViewController *)mainViewController {
  return (TKNMainViewController *)self.contentViewController;
}

- (TKNTypefaceViewController *)typefaceViewController {
  return self.mainViewController.typefaceViewController;
}

- (TKNSettingsViewController *)settingsViewController {
  return self.mainViewController.settingsViewController;
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
  [self.typefaceViewController zoomIn:sender];
}

- (void)zoomOut:(id)sender {
  [self.typefaceViewController zoomOut:sender];
}

- (void)installAdobeFDK:(id)sender {
  self.welcomeSheetController = [[TKNWelcomeSheetController alloc] init];
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
    NSString *applicationSupportPath = TKNFilePath.privateApplicationSupport;
    if ([fileManager fileExistsAtPath:applicationSupportPath]) {
      if (![fileManager removeItemAtPath:applicationSupportPath error:&error]) {
        [[NSAlert alertWithError:error]
            beginSheetModalForWindow:NSApp.mainWindow
            completionHandler:nil];
        return;
      }
    }
    NSString *libraryPath = TKNFilePath.privateLibrary;
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
