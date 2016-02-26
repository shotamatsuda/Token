//
//  TKNMainWindowController.h
//
//  The MIT License
//
//  Copyright (C) 2015-2016 Shota Matsuda
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

#import "Token-Swift.h"
#import "TKNTypeface.h"

@interface TKNMainWindowController : NSWindowController <NSWindowDelegate>

#pragma mark View Controllers

@property (nonatomic, strong, readonly, nonnull)
    TKNMainViewController *mainViewController;
@property (nonatomic, strong, readonly, nonnull)
    TKNTypefaceViewController *typefaceViewController;
@property (nonatomic, strong, readonly, nonnull)
    TKNSettingsViewController *settingsViewController;

#pragma mark Parameters

@property (nonatomic, strong, nonnull) TKNTypeface *typeface;

#pragma mark Actions

- (IBAction)exportFont:(nullable id)sender;
- (IBAction)installFont:(nullable id)sender;
- (IBAction)zoomIn:(nullable id)sender;
- (IBAction)zoomOut:(nullable id)sender;
- (IBAction)installAdobeFDK:(nullable id)sender;
- (IBAction)uninstallAdobeFDK:(nullable id)sender;

@end
