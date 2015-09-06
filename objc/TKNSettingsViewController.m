//
//  TKNSettingsViewController.m
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

#import "TKNSettingsViewController.h"

@interface TKNSettingsViewController ()

@property (nonatomic, strong) IBOutlet NSLayoutConstraint *capsHeightConstraint;
@property (nonatomic, strong) IBOutlet NSLayoutConstraint *strokeWidthConstraint;

@end

@implementation TKNSettingsViewController

- (instancetype)init {
  return [self initWithNibName:@"TKNSettingsViewController"
                        bundle:[NSBundle mainBundle]];
}

- (void)viewDidLoad {
  [super viewDidLoad];
  NSProcessInfo *processInfo = [NSProcessInfo processInfo];
  NSOperatingSystemVersion version = processInfo.operatingSystemVersion;
  if (version.majorVersion >= 10 && version.minorVersion >= 11) {
    _capsHeightConstraint.constant = -4.0;
    _strokeWidthConstraint.constant = -4.0;
  }
}

@end
