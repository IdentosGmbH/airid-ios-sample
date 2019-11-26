//
//  AIDCard.h
//  Test
//
//  Created by Wolfgang Sebastian Blaum on 16.07.14.
//  Updated by several people since then.
//  Copyright (c) 2014 Unicept GmbH. All rights reserved.
//  Copyright (c) 2017, 2018, 2019 certgate GmbH. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AirIDDriver/pcsclite.h>
#import "AIDDefines.h"

typedef NS_OPTIONS(NSUInteger, AIPCardProtocol){
    AIPCardProtocolUndefined = 0,
    AIPCardProtocolT0 = (1 << 0),
    AIPCardProtocolT1 = (1 << 1),
    AIPCardProtocolTx = (AIPCardProtocolT0 | AIPCardProtocolT1),
    AIPCardProtocolRaw = (1 << 4),
};

NS_ASSUME_NONNULL_BEGIN

typedef void (^AIDCardAPDUCompletionHandler)(NSData * _Nullable receivedData, SCARD_IO_REQUEST * _Nullable receiveProtocol, NSError * _Nullable error);
typedef void (^AIDCardResetCompletionHandler)(NSData * _Nullable receivedData, NSError *error);
typedef void (^AIDCardShutdownCompletionHandler)(NSError *error);

@class AIDCard;

@class AIDDevice;

@interface AIDCard : NSObject

@property AIPCardProtocol activeProtocol;
/* AIDDevice also references AIDCard, to avoid circular dependencies,
   use only a weak reference here */
@property(weak, nonatomic) AIDDevice *device;

- (instancetype)init __attribute__((unavailable("This is not the designated initializer for AIDCard")));
- (instancetype)initWithDevice:(AIDDevice *)device NS_DESIGNATED_INITIALIZER;
- (void)sendAPDUWithData:(NSData *)data withIORequest:(nullable const SCARD_IO_REQUEST *)request completion:(AIDCardAPDUCompletionHandler)callback;
- (void)resetCardWithCompletion:(nullable AIDCardResetCompletionHandler)callback;
- (void)shutdownCardWithCompletion:(nullable AIDCardShutdownCompletionHandler)callback;
- (BOOL)beginTransaction;
- (void)endTransaction;

AID_EXTERN NSString *const AIDCardErrorDomain;

typedef NS_ENUM(NSUInteger, AIDCardError) {
    AIDCardErrorUnknown = 0,
    AIDCardErrorSharingViolation = 1,
};

NS_ASSUME_NONNULL_END

@end
