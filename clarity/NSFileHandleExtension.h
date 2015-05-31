//
//  NSFileHandleExtension.h
//  clarity
//
//  Created by Scott Douglas on 5/30/15.
//  Copyright (c) 2015 Scott Douglas. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface NSFileHandle (isReadableAddon)
- (BOOL) isReadable;
@end


BOOL isFileDescriptorReadable(int fd);