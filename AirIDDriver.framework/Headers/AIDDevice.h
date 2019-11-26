//
//  AIDDevice.h
//  AirIDDriver
//
//  Created by Wolfgang Sebastian Blaum on 22.07.14.
//  Updated by several people since then.
//  Copyright (c) 2014 Unicept GmbH. All rights reserved.
//  Copyright (c) 2017, 2018, 2019 certgate GmbH. All rights reserved.
//

#ifndef AID_DEVICE_H
#define AID_DEVICE_H
#import <CoreBluetooth/CoreBluetooth.h>
#import "AIDDefines.h"
#import "AIDDevicePairedDevices.h"

typedef NS_ENUM(uint8_t, RPCReturnCode) {
    RPCReturnCodeSuccess                     = 0x00,
    RPCReturnCodeMethodNotSpecified          = 0x01,
    RPCReturnCodeUnknownMethod               = 0x02,
    RPCReturnCodeMethodFailure               = 0x03,
    RPCReturnCodeDecryptFailure              = 0x04,
    RPCReturnCodeEncryptFailure              = 0x05,
    RPCReturnCodeKeyNotSet                   = 0x06,
    RPCReturnCodeIncorrectEncryption         = 0x07,
    RPCReturnCodeEncryptedMethodNotSpecified = 0x08,
};

typedef NS_ENUM(NSUInteger, AIDCardStatus)
{
    AIDCardStatusUnknown    = 0,
    AIDCardStatusAbsent     = 1,
    AIDCardStatusPresent    = 2,
    AIDCardStatusInPosition = 3,
    AIDCardStatusPowered    = 4,
    AIDCardStatusNegotiable = 5,
    AIDCardStatusSpecific   = 6, // The card is ready to use if it reaches this status
};

typedef NS_ENUM(NSInteger, AIDDeviceError)
{
    AIDDeviceErrorUnknown                   = 0,
    AIDDeviceErrorProcedureCall             = 1, // The AirID device received an unknown or invalid RPC call
    AIDDeviceErrorBluetooth                 = 2, // An operation failed because of a missing bluetooth connection
    AIDDeviceErrorEncryptionKeysMismatch    = 3, // Device and AirID have different keys for encryption
    AIDDeviceErrorBluetoothCommunication    = 4, // An error was reported by the bluetooth subsystem
    AIDDeviceErrorUnexpectedConnectionAbort = 5, // The connection to the device was aborted unexpectedly
    AIDDeviceErrorInitializationTimeout     = 6, // Initialization of a connected AirID device timed out
    AIDDeviceErrorDidFailToConnectDevice    = 7, // Bluetooth connection did fail to connect peripheral
};

typedef NS_ENUM(NSUInteger, AIDDeviceStatus)
{
    AIDDeviceStatusAbsent,      // The device is known by the system but sends no advertisment packets (or is out of range)
    AIDDeviceStatusPresent,     // The device is sending advertisment packets 
    AIDDeviceStatusConnected,   // The device was connected and is initalized at the moment
    AIDDeviceStatusInitialized, // The device is ready to use
};

/**
 The first digit of the serial number shows which device family is used.
 Use serialNumberToFamily() to get the type for a number.
*/
typedef NS_ENUM(NSUInteger, AIDDeviceFamily)
{
    AIDDeviceFamilyInvalid = 0, // Device family could not be parsed from ill-formed serial number.

    AIDDeviceFamilyClassic = 5, // AirID 1
    AIDDeviceFamilyDongle  = 6, // OK Bridge
    AIDDeviceFamilyMini    = 7, // xenox - CHECKME - can be reused ?
    AIDDeviceFamilyMicro   = 8, // xenox - CHECKME - can be reused ?
    AIDDeviceFamilyTwo     = 9, // xenox - CHECKME - it is AID2 really
    AIDDeviceFamilyThree   = 10, // xenox - TBD - just preparing
    AIDDeviceFamilyOKIDOne   = 11, // xenox - NOT SUPPORTED but will be reported
    AIDDeviceFamilyTwoMini   = 12, // xenox - A2MX is based on our first common hardware/firmware platform
    AIDDeviceFamilyThreeMini = 13  // xenox - A3MX - just preparing
};

@class CBPeripheral, AIDService, AIDDevice, AIDDeviceDescriptor;

@class AIDCard, AIDDeviceData;

NS_ASSUME_NONNULL_BEGIN

@interface NSData(ReturnCode)
- (RPCReturnCode)returnCode;
- (NSData*)firstByte;
@end

@interface AIDDevice : NSObject

/**
 Unique indentifier.
 */
@property(strong, nonatomic, readonly) NSUUID *identifier;

/**
 Name of the device. Use this for displaying purposes.
 */
@property(strong, nonatomic, readonly) NSString *name;

/**
 Averaged signal strength
 */
@property(strong, nonatomic, readonly) NSNumber *signalStrength;

/**
 Keys are automatically exchanged if needed. You can observe this property to show a notice to the user.
 */
@property(nonatomic, getter=isExchangingKeys) BOOL exchangingKeys;

/**
 True if the USB-Cable is plugged in at the moment.
 */
@property(nonatomic, readonly, getter=isCablePlugged) BOOL cablePlugged;

/**
 True if communication is secured by a 265-bit key.
 */
@property(nonatomic, readonly, getter=isEncryptionEnabled) BOOL encryptionEnabled;

/**
 The status of the smard card which is inserted into device. If Status changes to AIDCardStatusSpecific the AIDCard-property of the device will be non-nil and can be used to communicate with the smard card.
 */
@property(atomic, readonly) AIDCardStatus cardStatus;

/**
 Non-nil if the inserted card is ready to communicate.
 */
@property(strong, nonatomic, nullable) AIDCard *card;

/**
 Current status of the device.
 */
@property(nonatomic, readonly) AIDDeviceStatus status;

@end

/**
 Initialization is executed after a device is successfully connected. The properties in this section are only valid if the corresponding InitOperation was executed.
 */

@interface AIDDevice (Initialization)

typedef NS_OPTIONS(uint16_t, AIDInitOperation){
    AIDInitOperationNone                    = 0,
    AIDInitOperationReadDeviceDescriptor    = 1 << 0,
    AIDInitOperationReadBatteryLevel        = 1 << 1,
    AIDInitOperationReadCardInfo            = 1 << 2,
    AIDInitOperationReadSettings            = 1 << 3,
    AIDInitOperationSetFinderName           = 1 << 4,
    AIDInitOperationSetTime                 = 1 << 5,
    AIDInitOperationGetWorkingTime          = 1 << 6,
    AIDInitOperation256BitKey              = 1 << 7,

    AIDInitOperationsAll = (AIDInitOperationReadDeviceDescriptor | AIDInitOperationReadBatteryLevel | AIDInitOperationReadCardInfo | AIDInitOperationReadSettings | AIDInitOperationSetTime | AIDInitOperationSetFinderName | AIDInitOperationGetWorkingTime | AIDInitOperation256BitKey)};

+ (AIDInitOperation)requiredInitialisationOperations;
+ (void)setRequiredInitialisationOperations:(AIDInitOperation)operations;

//AIDInitOperationReadDeviceDescriptor
/**
 * returns the hardware address for the peripheral
 */
@property(strong, nonatomic, readonly, nullable) NSString *hardwareAddress;

/**
 * returns the serial number for the peripheral
 */
@property(strong, nonatomic, readonly, nullable) NSString *serialNumber;

/**
 * returns the firmware version of the peripheral
 */
@property(strong, nonatomic, readonly, nullable) NSString *firmwareVersion;

/**
 * returns the hardware version/revision of the peripheral
 */
@property(strong, nonatomic, readonly, nullable) NSString *hardwareVersion;

/**
 * returns the build date of the peripheral // xenox - TODO - build date of what? hw, fw I assume the former one.
 */
@property(strong, nonatomic, readonly, nullable) NSString *buildDate;

/**
 * returns the boot loader version of the peripheral
 */
@property(strong, nonatomic, readonly, nullable) NSString *bootloaderVersion;

//AIDInitOperationReadCardInfo
@property(strong, nonatomic, readonly, nullable) NSString *cardOS DEPRECATED_ATTRIBUTE;

//AIDInitOperationReadBatteryLevel
@property(strong, nonatomic, readonly, nullable) NSNumber *batteryLevel;

//AIDInitOperationReadSettings
@property(strong, nonatomic, readonly, nullable) NSDictionary *deviceSettings;
- (void)changeDeviceSettingWithKey:(nonnull NSString *)key withValue:(nonnull NSNumber *)value;
- (void)getPairedDevices:(void(^)(AIDDevicePairedDevices *prefDev, NSError *error)) callback;
- (void)removePairedDevice:(u_int8_t) idx callback:(void(^)(AIDDevicePairedDevices *prefDev, NSError *error)) callback;

AID_EXTERN NSString *const kDeviceSettingsTypeSignalStrength;
AID_EXTERN NSString *const kDeviceSettingsTypeAdvertisingMode;
AID_EXTERN NSString *const kDeviceSettingsTypeDisplay;
AID_EXTERN NSString *const kDeviceSettingsTypeBacklight;
AID_EXTERN NSString *const kDeviceSettingsTypeBuzzer;
AID_EXTERN NSString *const kDeviceSettingsTypeContrast;
AID_EXTERN NSString *const kDeviceSettingsTypeEncryption;

// access date like this:
// [array[day]["start"|"end"]["day"|"hour"|"minute"] asInt]
//@property(copy, readwrite) NSMutableArray *workingTimes;

// getter to retrieve a copy of working times of the device. if not known, nil is returned.
- (NSMutableArray *)getWorkingTimes;



// set the working times attribute of this class (should be called when the working times are retrieved)
- (void)gotWorkingTimesFromDevice:(NSMutableArray *)newWorkingTimes;

// set working times of device and update it.
// return true on success, false otherwise
- (bool)updateWorkingTimesOnDevice:(NSMutableArray *) workingTimes;

// TODO: can/should this be a static/free function?
- (AIDDeviceFamily)serialNumberToFamily:(NSString*) serialNumber;

@end

NS_ASSUME_NONNULL_END

#endif
