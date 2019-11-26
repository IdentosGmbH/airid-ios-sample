//
//  AIDDeviceManager.h
//  AirIDDriver
//
//  Created by Wolfgang Sebastian Blaum on 17.07.14.
//  Updated by several people since then.
//  Copyright (c) 2014 Unicept GmbH. All rights reserved.
//  Copyright (c) 2017, 2018, 2019 certgate GmbH. All rights reserved.

#ifndef AIRID_DRIVER_H
#define AIRID_DRIVER_H

#import <Foundation/Foundation.h>
#import <CoreBluetooth/CoreBluetooth.h>

#import <AirIDDriver/AIDDefines.h>
#import <AirIDDriver/UICKeyChainStore.h>

NS_ASSUME_NONNULL_BEGIN

AID_EXTERN NSString *const AIDDriverVersion;

@class AIDDeviceManager, AIDService, AIDDevice;

@protocol AIDDeviceManagerDelegate<NSObject>
    
@optional

- (void)deviceManagerStatePowerOn:(AIDDeviceManager *)manager;
- (void)deviceManagerStatePowerOff:(AIDDeviceManager *)manager;

/**
 This is called before the user selected device is changed.

 This is not called after disconnecting the user selected device and subsequently connecting it as the user
 selected device has not changed (just the state changed).
 */
- (void)deviceManager:(AIDDeviceManager *)manager willChangeUserSelectedDevice:(AIDDevice *)device;

/**
 This is called after the user selected device is changed.
 */
- (void)deviceManager:(AIDDeviceManager *)manager didChangeUserSelectedDevice:(AIDDevice *)device;

/**
 This will be called each time the device list changes.
 See documentation for devices for details when the list changes.
 */
- (void)deviceManagerDidChangeDeviceList:(AIDDeviceManager *)manager;

/**
 * If the delegate implements this callback it will be called when forgetUserSelectedDevice was called.
 */
- (void)deviceManagerDidForgetUserSelectedDevice:(AIDDeviceManager *)manager;

/**
 If the delegate implements this method the framework will not show any disconnection error message by itself.
 */
- (void)deviceManager:(AIDDeviceManager *)manager didDisconnectDevice:(AIDDevice *)device error:(nullable NSError *)error;

/**
 Called if the device failed to connect in which case the error will be set.
 */
- (void)deviceManager:(AIDDeviceManager *)manager didFailToConnectDevice:(AIDDevice *)device error:(nullable NSError *)error;

@end

@interface AIDDeviceManager : NSObject

+ (instancetype)sharedManager;

@property(nullable, weak, nonatomic) id<AIDDeviceManagerDelegate> delegate;

/**
 Scan for devices which are in advertising mode.
 */
@property(nonatomic) BOOL scanForPeripherals;

/**
 Found devices. This will include all devices except for the currently connected device.
 Even those which are not sending advertisment packets any more (state is Absent).
 
 The device list can be observed with the deviceManagerDidChangeDeviceList: delegate and changes when
 - a new connection has been made to a device (it is not available for connections anymore) 
 - the connection to a device has been closed (it is now available for new connections)
 - a device other than the user selected device changes it's state from/to Absent 
 */
@property(strong, atomic, readonly) NSArray *devices;

/**
 The device which is connected or was lastly connected. It will be automatically dis-/re-connected when the app changes to back-/fore-ground.
 If autoConnectSavedDevice is set to YES, the framework tries to save/read the savedDevice to the Keychain and connects it on startup.
 */
@property(nullable, strong, nonatomic, readonly) AIDDevice *savedDevice;
@property(nonatomic) BOOL autoConnectSavedDevice;
@property(nullable, strong, nonatomic, readonly) UICKeyChainStore *keyChainStore;



/**
 True if the bluetooth subsystem is powered on. May change if the user turns off bluetooth (e.g. by using the the command center).
 */
@property(getter=isBluetoothPoweredOn) BOOL bluetoothPoweredOn;

/**
 Start up the bluetooth subsystem and try to connect to saved device from KeyChain. You can specify the Keychain Group.
 */
- (void)startWithAccessGroup:(nullable NSString *)accessGroup;
- (void)start;

/**
 Connect a device.
 When the connection attempt and the subsequent initialization of the device fails, the callback didFailToConnectDevice:device:error will be
 called with the error set.
 */
- (void)connectDevice:(AIDDevice *)device;

/**
 Disconnect a device.
 */
- (void)disconnectDevice:(AIDDevice *)device;

/**
 Forget the saved device by setting it to nil and removing it from the keychain.
*/
- (void)forgetSavedDevice;

/**
 Private: do not use.
 */
- (void)deviceChangedFromPresentIntoAbsent:(AIDDevice*) device;

@end

AID_EXTERN NSString *const AIDDeviceManagerStatePowerOn;
AID_EXTERN NSString *const AIDDeviceManagerStatePowerOff;
AID_EXTERN NSString *const AIDDeviceManagerWillChangeUserSelectedDevice; //userInfo contains old device
AID_EXTERN NSString *const AIDDeviceManagerDidChangeUserSelectedDevice;  //userInfo contains new device
AID_EXTERN NSString *const AIDDeviceManagerDidChangeDeviceList;          //userInfo is nil. Use [AIDDeviceManager devices] to get the list of devices.

AID_EXTERN NSString *const AIDDeviceManagerUserSelectedDeviceUserInfoKey;

AID_EXTERN NSString *const AIDDeviceErrorDomain;

NS_ASSUME_NONNULL_END

#endif
