//
//  AIDDevicePairedDevices.h
//  AirIDDriver
//
//  Created by Kai Gräper on 18.12.15.
//  Updated by several people since then.
//  Copyright (c) 2014 Unicept GmbH. All rights reserved.
//  Copyright (c) 2017, 2018, 2019 certgate GmbH. All rights reserved.
//

@interface AIDDevicePairedDeviceInfo : NSObject<NSCoding>

@property(nonatomic) u_int8_t ident;
@property(strong, nonatomic) NSString *name;

@end


@interface AIDDevicePairedDevices : NSObject<NSCoding>

@property(nonatomic) u_int8_t preferedDeviceID;
@property(strong, nonatomic) NSArray *devices;
- (id)initWithData:(NSData*) data;

@end
