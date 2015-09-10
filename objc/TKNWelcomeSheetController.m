//
//  TKNAFDKOSheetController.m
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

static NSString *TKNAFDKOURL =
    @"https://download.macromedia.com/pub/developer/opentype/FDK-25-MAC.zip";

@interface TKNWelcomeSheetController ()

@property (nonatomic, strong) IBOutlet NSProgressIndicator *progressIndicator;
@property (nonatomic, assign) double progress;
@property (nonatomic, strong) NSURLDownload *download;
@property (nonatomic, strong) NSTask *unzip;
@property (nonatomic, strong) NSURLResponse *downloadResponse;
@property (nonatomic, assign) long long downloadedDataLength;
@property (nonatomic, strong) NSString *archiveDirectory;
@property (nonatomic, strong) NSString *archivePath;

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

#pragma mark Actions

- (void)begin:(id)sender {
  _progressIndicator.indeterminate = YES;
  [_progressIndicator startAnimation:self];
  NSURL *url = [NSURL URLWithString:TKNAFDKOURL];
  NSURLRequest *request = [NSURLRequest requestWithURL:url];
  _download = [[NSURLDownload alloc] initWithRequest:request delegate:self];
}

- (void)cancel:(id)sender {
  [_download cancel];
  [_unzip terminate];
  NSWindow *window = self.window;
  [window.sheetParent endSheet:window returnCode:NSModalResponseCancel];
}

#pragma mark Extracting Archive

- (void)extractArchive {
  _progressIndicator.indeterminate = YES;
  [_progressIndicator startAnimation:self];
  __weak typeof(self) wself = self;
  _unzip = [[NSTask alloc] init];
  _unzip.launchPath = @"/usr/bin/unzip";
  _unzip.currentDirectoryPath = _archiveDirectory;
  _unzip.arguments = @[@"-qo", _archivePath];
  _unzip.terminationHandler = ^(NSTask * task) {
    [wself didExtractArchive];
  };
  [_unzip launch];
}

- (void)didExtractArchive {
  [_progressIndicator stopAnimation:self];
}

#pragma mark NSURLDownloadDelegate

- (void)download:(NSURLDownload *)download didFailWithError:(NSError *)error {
  [[NSAlert alertWithError:error]
      beginSheetModalForWindow:[NSApplication sharedApplication].mainWindow
      completionHandler:nil];
}

- (void)downloadDidBegin:(NSURLDownload *)download {
  NSLog(@"downloadDidBegin");
}

- (void)downloadDidFinish:(NSURLDownload *)download {
  NSLog(@"downloadDidFinish");
  [self extractArchive];
}

- (void)download:(NSURLDownload *)download
    decideDestinationWithSuggestedFilename:(NSString *)filename {
  NSBundle *bundle = [NSBundle mainBundle];
  NSString *directory = [NSSearchPathForDirectoriesInDomains(
      NSApplicationSupportDirectory, NSUserDomainMask, YES).firstObject
          stringByAppendingPathComponent:bundle.bundleIdentifier];
  NSFileManager *fileManager = [NSFileManager defaultManager];
  NSError *error = nil;
  if (![fileManager fileExistsAtPath:directory]) {
    if (![fileManager createDirectoryAtPath:directory
                withIntermediateDirectories:YES
                                 attributes:nil
                                      error:&error]) {
      [[NSAlert alertWithError:error]
          beginSheetModalForWindow:[NSApplication sharedApplication].mainWindow
          completionHandler:nil];
    }
  }
  NSString *path = [directory stringByAppendingPathComponent:filename];
  [download setDestination:path allowOverwrite:YES];
  _archiveDirectory = directory;
  _archivePath = path;
}

- (void)download:(NSURLDownload *)download
    didReceiveResponse:(NSURLResponse *)response {
  _downloadResponse = response;
  _downloadedDataLength = 0;
  _progressIndicator.indeterminate = NO;
}

- (void)download:(NSURLDownload *)download
    didReceiveDataOfLength:(NSUInteger)length {
  _downloadedDataLength += length;
  long long expectedLength = _downloadResponse.expectedContentLength;
  if (expectedLength != NSURLResponseUnknownLength) {
    self.progress = (double)_downloadedDataLength / expectedLength;
  }
}

@end
