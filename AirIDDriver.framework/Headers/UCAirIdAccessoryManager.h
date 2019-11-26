//
//  UCAirIdAccessoryManager.h
//  AirIDDriver
//
//  Created by Wolfgang Sebastian Blaum on 17.07.14.
//  Updated by several people since then.
//  Copyright (c) 2014 Unicept GmbH. All rights reserved.
//  Copyright (c) 2017, 2018, 2019 certgate GmbH. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "UCAirIdAccessory.h"

@protocol UCAirIdAccessoryManagerDelegate;

DEPRECATED_MSG_ATTRIBUTE("Will be replaced by AIDDeviceManager")
@interface UCAirIdAccessoryManager : NSObject

+ (UCAirIdAccessory *)autoConnectAccessory;
- (id)initWithDelegate:(id<UCAirIdAccessoryManagerDelegate>)delegate;
- (id)initWithDelegate:(id<UCAirIdAccessoryManagerDelegate>)delegate andSelectedChangeFlags:(NSInteger)selectedChangeFlags;
- (void)connectAccessory:(NSString *)accessoryName;
- (void)disconnectAccessory:(NSString *)accessoryName;

@end

DEPRECATED_MSG_ATTRIBUTE("Will be replaced by AIDDeviceManagerDelegate")
@protocol UCAirIdAccessoryManagerDelegate<NSObject>

@required
- (void)airIdAccessoryManager:(UCAirIdAccessoryManager *)manager didDiscoverAccessory:(UCAirIdAccessory *)airIdAccessory;
- (void)airIdAccessoryManager:(UCAirIdAccessoryManager *)manager didUndiscoverAccessory:(UCAirIdAccessory *)airIdAccessory;
- (void)airIdAccessoryManager:(UCAirIdAccessoryManager *)manager didUpdateAccessory:(UCAirIdAccessory *)airIdAccessory changeFlags:(UCAccessoryChangeFlag)changeFlags;

@end
