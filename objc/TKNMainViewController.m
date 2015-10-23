//
//  TKNMainViewController.m
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

#import "TKNMainViewController.h"

@interface TKNMainViewController ()

@property (nonatomic, strong, nonnull)
    TKNTypefaceViewController *typefaceViewController;
@property (nonatomic, strong, nonnull)
    TKNSettingsViewController *settingsViewController;

@end

@implementation TKNMainViewController

- (void)prepareForSegue:(NSStoryboardSegue *)segue sender:(id)sender {
  [super prepareForSegue:segue sender:sender];
  if ([segue.identifier isEqualToString:@"Typeface"]) {
    self.typefaceViewController = segue.destinationController;
  } else if ([segue.identifier isEqualToString:@"Settings"]) {
    self.settingsViewController = segue.destinationController;
  }
}

- (void)viewDidLoad {
  [super viewDidLoad];
  NSString *defaultAppearance = NSAppearanceNameAqua;
  NSString *darkAppearance = NSAppearanceNameVibrantDark;
  _typefaceViewController.view.appearance =
      [NSAppearance appearanceNamed:defaultAppearance];
  _settingsViewController.view.appearance =
      [NSAppearance appearanceNamed:darkAppearance];
}

@end
