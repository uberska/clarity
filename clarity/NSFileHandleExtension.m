//
//  NSFileHandleExtension.m
//  clarity
//
//  Created by Scott Douglas on 5/30/15.
//  Copyright (c) 2015 Scott Douglas. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NSFileHandleExtension.h"


@implementation NSFileHandle (isReadableAddon)

- (BOOL) isReadable {
	return isFileDescriptorReadable([self fileDescriptor]);
}

@end


BOOL isFileDescriptorReadable(int fd) {
	// http://stackoverflow.com/questions/7505777/how-do-i-check-for-nsfilehandle-has-data-available
	fd_set fdset;
	struct timeval tmout = { 0, 0 }; // return immediately
	FD_ZERO(&fdset);
	FD_SET(fd, &fdset);
	return (select(fd + 1, &fdset, NULL, NULL, &tmout) > 0);
}