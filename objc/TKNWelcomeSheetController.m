//
//  TKNWelcomeSheetController.m
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

#import "TKNWelcomeSheetController.h"

#import "Token-Swift.h"

static NSString * const TKNAdobeFDKURL =
    @"https://download.macromedia.com/pub/developer/opentype/FDK-25-MAC.zip";

@interface TKNWelcomeSheetController ()

@property (nonatomic, strong) IBOutlet NSView *welcomeView;
@property (nonatomic, strong) IBOutlet NSView *licenseView;
@property (nonatomic, strong) IBOutlet NSTextView *licenseTextView;
@property (nonatomic, strong) IBOutlet NSView *progressView;

- (void)transitionToView:(NSView *)view
               resizable:(BOOL)resizable
       completionHandler:(dispatch_block_t)completionHandler;
- (void)install;
- (void)cleanUp;

#pragma mark Progress

@property (nonatomic, assign) double progress;
@property (nonatomic, strong) IBOutlet NSProgressIndicator *progressIndicator;
@property (nonatomic, copy) NSString *progressMessage;

#pragma mark Downloading

@property (nonatomic, strong) NSURLDownload *download;
@property (nonatomic, strong) NSURLResponse *downloadResponse;
@property (nonatomic, assign) long long downloadedDataLength;

- (void)downloadArchive;

#pragma mark Extracting

@property (nonatomic, strong) NSTask *unzip;
@property (nonatomic, strong) NSString *archiveDirectory;
@property (nonatomic, strong) NSString *archivePath;

- (void)extractArchive;
- (void)didExtractArchive;

@end

@implementation TKNWelcomeSheetController

- (instancetype)init {
  NSString *path = [[NSBundle mainBundle]
      pathForResource:@"TKNWelcomeSheetController"
               ofType:@"nib"];
  self = [super initWithWindowNibPath:path owner:self];
  if (self) {
  }
  return self;
}

- (void)windowDidLoad {
  [super windowDidLoad];

  // Welcome view
  NSWindow *window = self.window;
  NSView *view = window.contentView;
  CGRect frame = window.frame;
  frame.size = _welcomeView.frame.size;
  [window setFrame:frame display:NO];
  window.styleMask = window.styleMask & ~NSResizableWindowMask;
  _welcomeView.frame = view.bounds;
  _welcomeView.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
  [view addSubview:_welcomeView];
}

- (void)transitionToView:(NSView *)view
               resizable:(BOOL)resizable
       completionHandler:(dispatch_block_t)completionHandler {
  NSWindow *window = self.window;
  if (resizable) {
    window.styleMask = window.styleMask | NSResizableWindowMask;
  } else {
    window.styleMask = window.styleMask & ~NSResizableWindowMask;
  }
  [window.contentView addSubview:view];
  CGRect windowFrame = window.frame;
  windowFrame.size = view.frame.size;
  view.autoresizingMask = (NSViewMinXMargin | NSViewMaxXMargin |
                           NSViewMinYMargin);
  CGRect viewFrame = window.contentView.bounds;
  viewFrame.origin.x = (window.frame.size.width - view.frame.size.width) / 2.0;
  viewFrame.origin.y = (window.frame.size.height - view.frame.size.height);
  viewFrame.size = view.frame.size;
  view.frame = viewFrame;
  view.alphaValue = 0.0;
  NSView *contentView = window.contentView;
  NSView *currentView = contentView.subviews.firstObject;
  currentView.autoresizingMask = (NSViewMinXMargin | NSViewMaxXMargin |
                                  NSViewMinYMargin);
  [NSAnimationContext runAnimationGroup:^(NSAnimationContext *context) {
    view.animator.alphaValue = 1.0;
    currentView.animator.alphaValue = 0.0;
  } completionHandler:^{
    [currentView removeFromSuperview];
    view.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
    if (completionHandler) {
      completionHandler();
    }
  }];
  [window setFrame:windowFrame display:YES animate:YES];
}

- (void)install {
  self.progressMessage = NSLocalizedString(@"Installing...", @"");
  NSString *libraryPath = TKNFilePath.privateLibrary;
  NSString *linkPath = TKNFilePath.adobeFDK;
  NSString *path = [_archiveDirectory stringByAppendingPathComponent:@"FDK"];
  NSFileManager *fileManager = [NSFileManager defaultManager];
  NSError *error = nil;
  if (![fileManager fileExistsAtPath:libraryPath]) {
    if (![fileManager createDirectoryAtPath:libraryPath
                withIntermediateDirectories:YES
                                 attributes:nil
                                      error:&error]) {
      [[NSAlert alertWithError:error]
          beginSheetModalForWindow:NSApp.mainWindow
          completionHandler:nil];
      return;
    }
  } else if ([fileManager fileExistsAtPath:linkPath]) {
    if (![fileManager removeItemAtPath:linkPath error:&error]) {
      [[NSAlert alertWithError:error]
          beginSheetModalForWindow:NSApp.mainWindow
          completionHandler:nil];
      return;
    }
  }
  if (![fileManager createSymbolicLinkAtPath:linkPath
                         withDestinationPath:path
                                       error:&error]) {
    [[NSAlert alertWithError:error]
        beginSheetModalForWindow:NSApp.mainWindow
        completionHandler:nil];
    return;
  }
}

- (void)cleanUp {
  self.progressMessage = NSLocalizedString(@"Cleaning up...", @"");
  NSString *garbagePath = [_archiveDirectory
      stringByAppendingPathComponent:@"__MACOSX"];
  NSFileManager *fileManager = [NSFileManager defaultManager];
  NSError *error = nil;
  if ([fileManager fileExistsAtPath:garbagePath]) {
    if (![fileManager removeItemAtPath:garbagePath error:&error]) {
      [[NSAlert alertWithError:error]
          beginSheetModalForWindow:NSApp.mainWindow
          completionHandler:nil];
      return;
    }
  }
  if ([fileManager fileExistsAtPath:_archivePath]) {
    if (![fileManager removeItemAtPath:_archivePath error:&error]) {
      [[NSAlert alertWithError:error]
          beginSheetModalForWindow:NSApp.mainWindow
          completionHandler:nil];
      return;
    }
  }
}

#pragma mark Actions

- (void)begin:(id)sender {
  _licenseTextView.enclosingScrollView.hidden = YES;
  [self transitionToView:_licenseView resizable:YES completionHandler:^{
    NSBundle *bundle = [NSBundle mainBundle];
    NSData *data = [NSData dataWithContentsOfFile:
        [bundle pathForResource:@"AdobeFDKLicense" ofType:@"rtf"]];
    NSAttributedString *contents = [[NSAttributedString alloc]
        initWithRTF:data
        documentAttributes:NULL];
    _licenseTextView.textStorage.attributedString = contents;
    _licenseTextView.textContainerInset = CGSizeMake(0.0, 8.0);
    _licenseTextView.enclosingScrollView.hidden = NO;
  }];
}

- (void)acceptLicenseAgreement:(id)sender {
  [self transitionToView:_progressView resizable:NO completionHandler:^{
    [_progressIndicator startAnimation:self];
    [self downloadArchive];
  }];
}

- (void)declineLicenseAgreement:(id)sender {
  [self cancel:sender];
}

- (void)cancel:(id)sender {
  [_download cancel];
  [_unzip terminate];
  NSWindow *window = self.window;
  [window.sheetParent endSheet:window returnCode:NSModalResponseCancel];
}

#pragma mark Downloading

- (void)downloadArchive {
  NSURL *url = [NSURL URLWithString:TKNAdobeFDKURL];
  NSURLRequest *request = [NSURLRequest requestWithURL:url];
  self.download = [[NSURLDownload alloc] initWithRequest:request delegate:self];
}

- (void)download:(NSURLDownload *)download didFailWithError:(NSError *)error {
  [[NSAlert alertWithError:error]
      beginSheetModalForWindow:NSApp.mainWindow
      completionHandler:nil];
}

- (void)downloadDidBegin:(NSURLDownload *)download {
  _progressIndicator.indeterminate = NO;
  self.progressMessage = NSLocalizedString(@"Downloading...", @"");
}

- (void)downloadDidFinish:(NSURLDownload *)download {
  [self extractArchive];
}

- (void)download:(NSURLDownload *)download
    decideDestinationWithSuggestedFilename:(NSString *)filename {
  NSString *directory = TKNFilePath.privateApplicationSupport;
  NSFileManager *fileManager = [NSFileManager defaultManager];
  NSError *error = nil;
  if (![fileManager fileExistsAtPath:directory]) {
    if (![fileManager createDirectoryAtPath:directory
                withIntermediateDirectories:YES
                                 attributes:nil
                                      error:&error]) {
      [[NSAlert alertWithError:error]
          beginSheetModalForWindow:NSApp.mainWindow
          completionHandler:nil];
      return;
    }
  }
  NSString *path = [directory stringByAppendingPathComponent:filename];
  [download setDestination:path allowOverwrite:YES];
  self.archiveDirectory = directory;
  self.archivePath = path;
}

- (void)download:(NSURLDownload *)download
    didReceiveResponse:(NSURLResponse *)response {
  self.downloadResponse = response;
  self.downloadedDataLength = 0;
}

- (void)download:(NSURLDownload *)download
    didReceiveDataOfLength:(NSUInteger)length {
  self.downloadedDataLength += length;
  long long expectedLength = _downloadResponse.expectedContentLength;
  if (expectedLength != NSURLResponseUnknownLength) {
    self.progress = (double)_downloadedDataLength / expectedLength;
  }
}

#pragma mark Extracting

- (void)extractArchive {
  self.progressMessage = NSLocalizedString(@"Extracting...", @"");
  _progressIndicator.indeterminate = YES;
  [_progressIndicator startAnimation:self];
  self.unzip = [[NSTask alloc] init];
  _unzip.launchPath = @"/usr/bin/unzip";
  _unzip.currentDirectoryPath = _archiveDirectory;
  _unzip.arguments = @[@"-qo", _archivePath];
  __weak typeof(self) wself = self;
  _unzip.terminationHandler = ^(NSTask * task) {
    [wself didExtractArchive];
  };
  [_unzip launch];
}

- (void)didExtractArchive {
  // Will be invoked in a background thread
  dispatch_async(dispatch_get_main_queue(), ^{
    [self install];
    [self cleanUp];
    [_progressIndicator stopAnimation:self];
    NSWindow *window = self.window;
    [window.sheetParent endSheet:window returnCode:NSModalResponseOK];
  });
}

@end
