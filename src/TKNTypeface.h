// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#import <AppKit/AppKit.h>

typedef void (^TKNTypefaceProgressHandler)(NSUInteger, NSUInteger);

@interface TKNTypeface : NSObject

- (void)createFontWithContentsOfURL:(nonnull NSURL *)contentsURL
    toURL:(nonnull NSURL *)fontURL
    toolsURL:(nonnull NSURL *)toolsURL
    extraURL:(nonnull NSURL *)extraURL
    progressHandler:(nullable TKNTypefaceProgressHandler)progressHandler
    completionHandler:(nullable dispatch_block_t)completionHandler;
- (BOOL)correctUPEM:(double)UPEM
       forFontAtURL:(nonnull NSURL *)fontURL
           toolsURL:(nonnull NSURL *)toolsURL;

@end
