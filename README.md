# airid-ios-sample
AirID iOS example to use AirID framework

Initial Revision

AirIDDriver HowTo for iOS 

Using API
Use the framework
1. Add the framework to the "Embedded Binaries"-Section in the General-Tab of the Project-Settings.
2. Add in info.plist  NSBluetoothAlwaysUsageDescription

        <key>NSBluetoothAlwaysUsageDescription</key>
	<string>App communicates using CoreBluetooth</string>
	<key>NSBluetoothPeripheralUsageDescription</key>
	<string>$(PRODUCT_NAME) requires Bluetooth</string>
	<key>UIBackgroundModes</key>
	<array>
		<string>bluetooth-central</string>
	</array>

3. Import the AirIDDriver umbrella header wherever you want to use the SDK.

#import <AirIDDriver/AirIDDriver.h> 
The device manager gives you the opportunity to search for AirIDs and connect to a device. Call [[AIDDeviceManager sharedManager] setScanForPeripherals:YES] before calling 
[[AIDDeviceManager sharedManager] start] to tell the device manager to scan for AirIDs. To be notified when devices appear or disappear either set the delegate property of the device manager or register the AIDDeviceManagerDidChangeDeviceList notification in 
NSNotificationCenter . Here is an example: 
- (void)startDeviceManagerScanningForDevices
{
    //set yourself as delegate
    AIDDeviceManager.sharedManager.delegate = self;
    
    AIDDeviceManager.sharedManager.autoConnectSavedDevice = NO;
    [[AIDDeviceManager sharedManager] forgetSavedDevice];
}
If the BLE activated (poor on) start scan for peripherals
- (void)deviceManagerStatePowerOn:(AIDDeviceManager *)manager
{
    // start peripherals scanning...
    [[AIDDeviceManager sharedManager] setScanForPeripherals:YES];
    // initialize AIDDeviceManager
    [[AIDDeviceManager sharedManager] start];
}

Connecting with the SmartCard 
When successfully connected and initialized with the AirID reader
 you can either use the winscard inspired API in AirIdSCard.h 
or the card-property of AIDDevice to talk to the SmartCard. 
The latter is recommended and shown here. 
First you have to reset the card. This will power on the card if it's not already. 

After that you can send APDUs to the card by calling [AIDCard sendAPDUWithData: 
(NSData* receivedData, SCARD_IO_REQUEST* receiveProtocol))callback;] .
 Use NULL for the SCARD_IO_REQUEST as protocol-testing is not implemented yet. 
The AirID assumes usage of the first reported protocol if multiple are supported. 
