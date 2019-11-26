//
//  ViewController.m
//  AirIDSampleApp
//
//  Created by istvan czobel on 21.10.19.
//  Copyright © 2019 istvan czobel. All rights reserved.
//
/*
 * Copyright (c) 2019 Identos GmbH
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Precise Biometrics AB nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 */

#import "ViewController.h"
#import <AirIDDriver/AirIDDriver.h>
#import "PBSmartcardUtilities.h"

@interface ViewController () <AIDDeviceManagerDelegate>

@property (strong, atomic, readonly)  AIDDevice *myDevice;
@property (strong, atomic, readonly) NSString *deviceName;
@property (strong, atomic, readonly) NSUUID *deviceIdentifier;
@property (strong, atomic, readonly) NSString *deviceStatus;
@property (strong, atomic, readonly) NSString *deviceSerNb;
@property (strong, atomic, readonly) NSString *deviceCardStatus;

@property ( readwrite, assign ) NSInteger deviceIsConnected;

@end

NSString *keychainAccessGroup = @"my keychain-access-group";

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    [self->_disconn setTitle:@"connect" forState:UIControlStateNormal];
    _deviceIsConnected = 1;
    [self startDeviceManagerScanningForDevices];
}

- (IBAction)setConnect_Disconnect:(id)sender
{
    if(_deviceIsConnected == 1)
    {
        [self->_disconn setTitle:@"connect" forState:UIControlStateNormal];
        self.stringLabel.text = @"Device";
        self.statusStringLabel.text = @"Start scan to connect";
        self.statusSCLabel.text = @"---";
        self.responseSCLabel.text = @"---";
        NSLog (@"Start scan to connect");
        [self startDeviceManagerScanningForDevices];
    }
    else
    {
        self.stringLabel.text = @"Device";
        self.statusStringLabel.text = @"---";
        self.statusSCLabel.text = @"---";
        self.responseSCLabel.text = @"---";
        [self->_disconn setTitle:@"disconnect" forState:UIControlStateNormal];
        NSLog (@"Disconnect device");
        //[self disconnectDevices];
        [[AIDDeviceManager sharedManager] disconnectDevice:self.myDevice];
    }
    
}

- (void)startDeviceManagerScanningForDevices
{
    //set yourself as delegate
    AIDDeviceManager.sharedManager.delegate = self;
    
    //(or)register for notification
    /*
     [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(deviceListChanged:)
     name:AIDDeviceManagerDidChangeDeviceList
     object:nil];
     */
    
    AIDDeviceManager.sharedManager.autoConnectSavedDevice = NO;
    [[AIDDeviceManager sharedManager] forgetSavedDevice];
    // will start after delegate self
    // AIDDeviceManager.sharedManager.scanForPeripherals = YES;
    // [AIDDeviceManager.sharedManager start];
}

- (void)disconnectDevices
{
    AIDDeviceManager.sharedManager.delegate = self;

    //AIDDeviceManager.sharedManager.autoConnectSavedDevice = NO;
    [[AIDDeviceManager sharedManager] forgetSavedDevice];
    _deviceIsConnected = 1;
}

- (void)centralManager:(CBCentralManager *)central didDiscoverPeripheral:(CBPeripheral *)peripheral advertisementData:(NSDictionary *)advertisementData RSSI:(NSNumber *)RSSI
{
    NSLog (@"%@", [NSString stringWithFormat:@"-%@", [advertisementData description]]);
    //NSString* data1 = advertisementData.RSSI
}

- (void)printDeviceDescriptor:(AIDDevice *)device
{
    if (device)
    {
        // example for retrieving AirID device descriptor informations
        NSLog (@"%@", [NSString stringWithFormat:@"RSSI: %@", [NSString stringWithFormat:@"%.0f", [[device signalStrength] doubleValue] * 100]]);
        NSLog (@"%@", [NSString stringWithFormat:@"Encryption: %@", device.isEncryptionEnabled ? @"256" : @"128"]);
        NSLog (@"%@", [NSString stringWithFormat:@"Key exchanging: %@", device.isExchangingKeys ? @"YES" : @"NO"]);
        NSLog (@"%@", [NSString stringWithFormat:@"Serial: %@", device.serialNumber]);
        NSLog (@"%@", [NSString stringWithFormat:@"Hardware Address: %@", device.hardwareAddress]);
        NSLog (@"%@", [NSString stringWithFormat:@"Software Version: %@", device.firmwareVersion]);
        NSLog (@"%@", [NSString stringWithFormat:@"Hardware Version: %@", device.hardwareVersion]);
        NSLog (@"%@", [NSString stringWithFormat:@"BuildDate: %@", device.buildDate]);
        NSLog (@"%@", [NSString stringWithFormat:@"Bootloader Version: %@", device.bootloaderVersion]);
        NSLog (@"%@", [NSString stringWithFormat:@"Cable plugged: %@", device.isCablePlugged ? @"YES" : @"NO"]);
    }
}
- (void)testSmartcardInDevice:(AIDDevice *)device
{
    if ([device cardStatus] != AIDCardStatusSpecific)
    {
        NSLog (@"%@", [NSString stringWithFormat:@"Smart-card has status:[%lu] 'is not ready'", (unsigned long)[device cardStatus]]);
        NSLog (@"Waiting for the smart-card...");
    
        //Use main queue if you change any UI
        dispatch_async (dispatch_get_main_queue (),
                        ^{
                            self.statusSCLabel.text = @"Waiting for the smart-card...";
                        });
       
        return;
    }
    
    AIDCard *card = [device card];
    
    if (card == nil)
    {
        @throw [NSException
                exceptionWithName:[NSString stringWithFormat:@"%@Error", self ]
                reason:@"AIDCard:card object is nil"
                userInfo:nil];
    }
    
    // reset card and retrieve ATR
    [card resetCardWithCompletion:^(NSData *_Nullable receivedData, NSError *_Nonnull error)
     {
         if (error)
         {
             @throw [NSException
                     exceptionWithName:[NSString stringWithFormat:@"%@Error", self]
                     reason:error.localizedDescription
                     userInfo:error.userInfo];
         }
         else
         {
             if (receivedData)
             {
                 NSLog (@"%@", [NSString stringWithFormat:@"Smart-card ATR: %@", receivedData]);
                 NSString *strATR = [NSString stringWithFormat:@"SC ATR: %@", receivedData];
                 //Use main queue if you change any UI
                 dispatch_async (dispatch_get_main_queue (),
                                 ^{
                                     self.statusSCLabel.text = strATR;
                                 });
                 
             }
             
             // [self finishedWithResult:nil];
         }
     }];
    
    ///////////////////////////////////////////////////////////////////////
    /* now you are able to use (void)sendAPDUWithData:(NSData *)data withIORequest:(nullable const SCARD_IO_REQUEST *)request completion:(AIDCardAPDUCompletionHandler)callback;
     */
    
    NSData *data = [PBSmartcardUtilities fromHexString:@"00 A4 04 0C 06 D2 76 00 00 01 02"];
    
    [card sendAPDUWithData:data withIORequest:NULL completion:^(NSData *_Nullable receivedData, SCARD_IO_REQUEST *_Nullable protocol,NSError *_Nonnull error)
     {
         //receivedData will contain the answer to your APDU
         if (receivedData)
         {
             NSLog (@"%@", [NSString stringWithFormat:@"Smart-card Res: %@", receivedData]);
             if ([PBSmartcardUtilities statusBytesFrom:receivedData] == 0x9000) {
                 NSLog(@"Command executed successfully: 0x%X", [PBSmartcardUtilities statusBytesFrom:receivedData]);
                 NSLog(@"eGK detected");
                 dispatch_async (dispatch_get_main_queue (),
                                 ^{
                                     self.responseSCLabel.text = @"eGK detected";
                                 });
             }
             else
             {
                 NSLog(@"not an eGK");
                 dispatch_async (dispatch_get_main_queue (),
                                 ^{
                                     self.responseSCLabel.text = @"not an eGK";
                                 });
                 //to do try other APPID from specific cards
                 // belga card austria ecard ...
             }
             
         }
         // [self finishedWithResult:nil];
     }];
    
    /////////////////////////////////////////////////////////////////////
    // shutdown / power down card if no longer needed (battery draining!)
    [card shutdownCardWithCompletion:^(NSError *_Nonnull error)
     {
         if (error)
         {
             @throw [NSException
                     exceptionWithName:[NSString stringWithFormat:@"%@Error", self]
                     reason:error.localizedDescription
                     userInfo:error.userInfo];
         }
         else
         {
             NSLog (@"%@", [NSString stringWithFormat:@"Smartcard successfully shutdown "]);
         }
     }];
}

- (void)checkChangedDeviceList:(AIDDevice *)previouslyConnectedDevice
{
    NSLog (@"%@", [NSString stringWithFormat:@"did change device list (checkChangedDeviceList) (%@on main thread)",
                   ([[NSThread currentThread] isMainThread] ? @"" : @"NOT ")]);
    
    NSArray *devices = AIDDeviceManager.sharedManager.devices;
    
    // note: this is not the best way, better is to check against preferred device name/identifier and if available in devices use that
    if (devices.count > 0)
    {
        //Do something with devices. Use [AIDDevice name] for displaying purposes.
        // save the first device
        AIDDevice *device = devices[0];
        _deviceName = device.name;
        _deviceSerNb = device.serialNumber;
        _deviceIdentifier = [device identifier];
        
        NSLog (@"device Name  : %@", _deviceName);
        //    NSLog(@"device status: %@",device.status);
        NSLog (@"device UUID: %@", _deviceIdentifier);
        //    NSLog(@"card status  : %@",device.cardStatus);
        _myDevice = device;
        
        //Use main queue if you change any UI
        dispatch_async (dispatch_get_main_queue (),
                        ^{
                            self.stringLabel.text = self.deviceName;
                        });
        
        printf ("Selected device:%s UUID:%s\n", [[_myDevice name] UTF8String], [[[_myDevice identifier] UUIDString] UTF8String]);
        
        [self connectMyDevice:_myDevice];
    }
}
- (BOOL)connectMyDevice:(AIDDevice *)device
{
    
    [device addObserver:self forKeyPath:@"status" options:(NSKeyValueObservingOptionInitial | NSKeyValueObservingOptionNew) context:nil];
    // stop peripherals scanning...
    [[AIDDeviceManager sharedManager] setScanForPeripherals:NO];
    [[AIDDeviceManager sharedManager] connectDevice:device];
    
    return TRUE;
    
}

#pragma mark - AIDDeviceManagerDelegate

- (void)deviceManagerDidChangeDeviceList:(AIDDeviceManager *)manager
{
    NSLog (@"deviceManagerDidChangeDeviceList CB triggered");
    
    for (AIDDevice *device in [manager devices])
    {
        printf ("In devices list -> Device:%s UUID:%s\n", [[device name] UTF8String], [[[device identifier] UUIDString] UTF8String]);
    }
    
    // if if devices contains our prefered device name/identifier
    [self checkChangedDeviceList:nil];
}
- (void)deviceManagerStatePowerOn:(AIDDeviceManager *)manager
{
    NSLog (@"deviceManagerStatePowerOn CB triggered");
    // don't rely on savedDevice
    [[AIDDeviceManager sharedManager] forgetSavedDevice];
    // start peripherals scanning...
    [[AIDDeviceManager sharedManager] setScanForPeripherals:YES];
    // initialize AIDDeviceManager
    [[AIDDeviceManager sharedManager] start];
}
- (void)deviceManagerStatePowerOff:(AIDDeviceManager *)manager
{
    NSLog (@"deviceManagerStatePowerOff CB triggered");
    
    @throw [NSException
            exceptionWithName:[NSString stringWithFormat:@"%@ Error", self]
            reason:@"The Bluetooth is powered Off"
            userInfo:nil];
}
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context
{
    NSLog (@"%@", [NSString stringWithFormat:@"AirID CB: observeValueForKeyPath CB triggered for keypath:%@, (%@on main thread)",
                   keyPath, ([[NSThread currentThread] isMainThread] ? @"" : @"NOT ")]);
    
    if ([object isKindOfClass:[AIDDevice class]])
    {
        AIDDevice *device = (AIDDevice *)object;
        
        if ([keyPath isEqualToString:@"status"])
        {
            // handling of AirID status
            switch ([device status])
            {
                case AIDDeviceStatusAbsent:
                {
                    NSLog (@"%@", @"Status changed to 'Device is absent'");
                    dispatch_async (dispatch_get_main_queue (), ^{
                        [self->_disconn setTitle:@"connect" forState:UIControlStateNormal];
                        self.statusStringLabel.text = @"Device is absent";
                        self->_deviceIsConnected = 1;
                    });
                    break;
                }
                    
                case AIDDeviceStatusPresent:
                {
                    NSLog (@"%@", @"Status changed to 'Device is present'");
                    dispatch_async (dispatch_get_main_queue (), ^{
                        [self->_disconn setTitle:@"disconnect" forState:UIControlStateNormal];
                        self.statusStringLabel.text = @"Device is present";
                    });
                    break;
                }
                    
                case AIDDeviceStatusConnected:
                {
                    NSLog (@"%@", @"Status changed to 'Device is connected'");
                    _deviceIsConnected = 0;
#if 1
                    // save and connect airID device
                    device = [[AIDDeviceManager sharedManager] savedDevice];
#endif
                    dispatch_async (dispatch_get_main_queue (), ^{
                        [self->_disconn setTitle:@"disconnect" forState:UIControlStateNormal];
                        self.statusStringLabel.text = @"Device is connected";
                    });
                    break;
                }
                    
                case AIDDeviceStatusInitialized:
                {
                    _deviceIsConnected = 0;//connected
                    NSLog (@"%@", @"Status changed to 'Device is initialized and ready to use'");
                    // Session encryption is initialized and now you can do something with the AirID device.
                    dispatch_async (dispatch_get_main_queue (), ^{
                        //   [self->_disconn setTitle:@"Device is initialized" forState:UIControlStateNormal];
                        self.statusStringLabel.text = @"Device is initialized";
                        
                    });
                    
                    AIDDevice *device = self.myDevice;
                    
                    if (device)
                    {
                        [self printDeviceDescriptor:device];
                        // register us as observer for card status
                        [device addObserver:self forKeyPath:@"cardStatus" options:(NSKeyValueObservingOptionInitial | NSKeyValueObservingOptionNew) context:nil];
                        
                    }
                    
                    break;
                }
            }
        }
        else if ([keyPath isEqualToString:@"cardStatus"])
        {
            switch ([device cardStatus])
            {
                case AIDCardStatusUnknown:
                    NSLog (@"Card status changed to: 'AIDCardStatusUnknown'");
                    self.statusSCLabel.text = @"Card unknow";
                    break;
                    
                case AIDCardStatusAbsent:
                    NSLog (@"Card status changed to: 'AIDCardStatusAbsent'");
                    self.statusSCLabel.text =@"Card absent";
                    break;
                    
                case AIDCardStatusPresent:
                    NSLog (@"Card status changed to: 'AIDCardStatusPresent'");
                    self.statusSCLabel.text =@"Card present";
                    break;
                    
                case AIDCardStatusInPosition:
                    NSLog (@"Card status changed to: 'AIDCardStatusInPosition'");
                    self.statusSCLabel.text =@"Card present";
                    break;
                    
                case AIDCardStatusPowered:
                    NSLog (@"Card status changed to: 'AIDCardStatusPowered'");
                    self.statusSCLabel.text =@"Card powered";
                    break;
                    
                case AIDCardStatusNegotiable:
                    NSLog (@"Card status changed to: 'AIDCardStatusNegotiable'");
                    self.statusSCLabel.text =@"Card Negotiable";
                    break;
                    
                case AIDCardStatusSpecific:
                    NSLog (@"Card status changed to: 'AIDCardStatusSpecific'");
                    NSLog (@"The card is ready to use");
                    // The card is ready to use if it reaches this!
                    // test smartcard functionality
                    [self testSmartcardInDevice:device];
                    
                    break;
            }
        }
    }
}
- (void)deviceManager:(AIDDeviceManager *)manager willChangeUserSelectedDevice:(AIDDevice *)device
{
    NSLog (@"%@", [NSString stringWithFormat:@"AirID CB: will change user selected device:%@, (%@on main thread)",
                   device.name, ([[NSThread currentThread] isMainThread] ? @"" : @"NOT ")]);
}
- (void)deviceManager:(AIDDeviceManager *)manager didChangeUserSelectedDevice:(AIDDevice *)device
{
    NSLog (@"%@", [NSString stringWithFormat:@"AirID CB: did change user selected device:%@ (%@on main thread)",
                   device.name, ([[NSThread currentThread] isMainThread] ? @"" : @"NOT ")]);
}
- (void)deviceManagerDidForgetUserSelectedDevice:(AIDDeviceManager *)manager
{
    NSLog (@"%@", [NSString stringWithFormat:@"AirID CB: device manager did forget user selected device (%@on main thread)",
                   ([[NSThread currentThread] isMainThread] ? @"" : @"NOT ")]);
}
- (void)deviceManager:(AIDDeviceManager *)manager didDisconnectDevice:(AIDDevice *)device error:(NSError *)error
{
    NSLog (@"AirID CB: did disconnect device , error: %@,  (%@on main thread)",
           (error != nil ? error : @"SUCCESS"), ([[NSThread currentThread] isMainThread] ? @"" : @"NOT "));
    
    if (error != nil)
    {
        NSLog (@"error domain: %@, code: %ld", error.domain, (long)error.code);
        NSLog (@"error userinfo: %@", error.userInfo);
    }
}
- (void)deviceManager:(AIDDeviceManager *)manager didFailToConnectDevice:(nonnull AIDDevice *)device error:(nullable NSError *)error
{
    NSLog (@"AirID CB: device manager did fail to connect to device %@ w/ error: %@ (%@on main thread)",
           device.name, error.localizedDescription, ([[NSThread currentThread] isMainThread] ? @"" : @"NOT "));
    NSLog (@"error domain: %@, code: %ld", error.domain, (long)error.code);
    NSLog (@"error userinfo: %@", error.userInfo);
}
/*XENOX*/

//"myDevice"" was set when we told DeviceManger to connect it.
- (void)applicationDidEnterBackground:(UIApplication *)application
{
    [[AIDDeviceManager sharedManager] disconnectDevice:self.myDevice];
}
- (void)applicationWillEnterForeground:(UIApplication *)application
{
    [[AIDDeviceManager sharedManager] connectDevice:self.myDevice];
}


@end

