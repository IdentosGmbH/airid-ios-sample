//
//  UCAirIdAccessory.h
//
//  Unicept AirID accessory manager
//
//  Created by Alireza Barzegar on 2014-03-28.
//  Updated by several people since then.
//  Copyright (c) 2014 Unicept GmbH. All rights reserved.
//  Copyright (c) 2017, 2018, 2019 certgate GmbH. All rights reserved.
//

#ifndef UC_AIRID_ACCESSORY_H
#define UC_AIRID_ACCESSORY_H

#import <Foundation/Foundation.h>

#ifndef UC_EXTERN
#ifdef __cplusplus
#define UC_EXTERN extern "C" __attribute__((visibility("default")))
#else
#define UC_EXTERN extern __attribute__((visibility("default")))
#endif
#endif

UC_EXTERN NSString *const UCAirIdAccessoryVersion;

extern NSString *const kDeviceStatusNotification;
extern NSString *const kCardStatusNotification;

typedef enum {
    UC_ACF_CONNECTION = 1 << 0,
    UC_ACF_SIGNALSTRENGTH = 1 << 1,
    UC_ACF_CARD = 1 << 2,
    UC_ACF_CARD_ATR = 1 << 3,
    UC_ACF_OCCUPATION = 1 << 4,
    UC_ACF_COMMUNICATION = 1 << 5,
    UC_ACF_ERROR = 1 << 6
} UCAccessoryChangeFlag;

#define UC_ACF_SUCCESS UC_ACF_ERROR

NSString *const stringFromUCAccessoryChangeFlags(NSInteger flags);

typedef enum {
    UC_ACCESSORY_DISCONNECTING,
    UC_ACCESSORY_DISCONNECTED,
    UC_ACCESSORY_CONNECTING,
    UC_ACCESSORY_CONNECTED,
} UCAccessoryConnectionStatus;

UC_EXTERN NSString *const stringFromUCAccessoryConnectionStatus(UCAccessoryConnectionStatus);

typedef enum {
    UC_CARD_UNKNOWN = 0,    //       Indicates that the reader driver has no information concerning the current state of the reader.
    UC_CARD_ABSENT = 1,     //       Indicates that there is no card in the reader.
    UC_CARD_PRESENT = 2,    //       Indicates that a card is present in the reader, but it has not been moved into position for use.
    UC_CARD_SWALLOWED = 3,  // (RFU) Indicates that a card is in the reader and in position for use. The card is not powered.
    UC_CARD_POWERED = 4,    // (RFU) Indicates that the card is powered, but the reader driver has no additional information concerning the state of the card.
    UC_CARD_NEGOTIABLE = 5, // (RFU) Indicates that the card has been reset and is awaiting PTS negotiation.
    UC_CARD_SPECIFIC = 6    //       Indicates that the card has been reset, and specific communication protocols have been established.
} UCAccessoryCardStatus;

//UC_EXTERN NSString * const stringFromUCAccessoryCardStatus(UCAccessoryCardStatus);

typedef enum {
    UC_OCCUPATION_FREE,
    UC_OCCUPATION_SHARED,
    UC_OCCUPATION_EXCLUSIVE,
} UCOccupationStatus;

//UC_EXTERN NSString * const stringFromUCOccupationStatus(UCOccupationStatus);

typedef enum {
    UC_TS_OFFLINE,
    UC_TS_READY,
    UC_TS_TRANSMITTING,
    UC_TS_RECEIVING,
    UC_TS_FAILED
} UCTranceiveStatus;

typedef enum {
    UCCacheStatusOff = 0,
    UCCacheStatusOn,
    UCCacheStatusOnWithContent
} UCCacheStatus;

UC_EXTERN NSString *const stringFromUCTranceiveStatus(UCTranceiveStatus);

/** This class holds all information about a AirID device */
DEPRECATED_MSG_ATTRIBUTE("Will be replaced by AIDDevice")
@interface UCAirIdAccessory : NSObject

@property(readonly) NSString *name;
@property(readonly) double signalStrength;
@property(readonly) UCAccessoryConnectionStatus connectionStatus;
@property(readonly) UCAccessoryCardStatus cardStatus;
@property(readonly) UCOccupationStatus occupationStatus;
@property(readonly) UCTranceiveStatus tranceiveStatus;
@property(readonly) NSString *errorMessage;
@property BOOL shouldAutoConnect;

@property(readonly) NSString *macAddress;
@property(readonly) NSString *serialNumber;
@property(readonly) NSString *firmwareVersion;
@property(readonly) NSString *hardwareVersion;
@property(readonly) NSString *cardOSVersion;

@property(readonly) int batteryLevel;

@property(readonly) NSString *displayName;

+ (NSString *)getAirIdNameFromKeyChain;
+ (NSString *)getAirIdMacAddressFromKeyChain;
+ (NSString *)getAirIdSerialFromKeyChain;

@end

#endif // ndef UC_AIRID_ACCESSORY_H
