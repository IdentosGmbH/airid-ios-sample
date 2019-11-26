//
//  AIDDeviceTableViewController.h
//  AirIDDriver
//
//  Created by Wolfgang Sebastian Blaum on 06.03.15.
//  Updated by several people since then.
//  Copyright (c) 2014 Unicept GmbH. All rights reserved.
//  Copyright (c) 2017, 2018, 2019 certgate GmbH. All rights reserved.
//

#import <UIKit/UIKit.h>

@class AIDDevicePickerController, AIDDevice;

NS_ASSUME_NONNULL_BEGIN

typedef void (^AIDDevicePickerCompletionHandler)(AIDDevicePickerController *devicePickerController, BOOL userDidSelect, NSError *__nullable error);

@protocol AIDDevicePickerControllerDelegate<NSObject>
@optional

- (nullable UIViewController *)devicePickerControllerParentViewController:(AIDDevicePickerController *)devicePickerController;
- (BOOL)devicePickerController:(AIDDevicePickerController *)devicePickerController shouldShowDevice:(AIDDevice *)device;
- (void)devicePickerControllerDidSelectDevice:(AIDDevicePickerController *)devicePickerController;

@end

@interface AIDDevicePickerController : NSObject

@property(nonatomic, assign, nullable) id<AIDDevicePickerControllerDelegate> delegate;

@property(strong, nonatomic, readonly, nullable) AIDDevice *selectedDevice;

- (BOOL)presentAnimated:(BOOL)animated completionHandler:(nullable AIDDevicePickerCompletionHandler)completion;                                             // iPhone
- (BOOL)presentFromRect:(CGRect)rect inView:(UIView *)view animated:(BOOL)animated completionHandler:(nullable AIDDevicePickerCompletionHandler)completion; // iPad
- (BOOL)presentFromBarButtonItem:(UIBarButtonItem *)item animated:(BOOL)animated completionHandler:(nullable AIDDevicePickerCompletionHandler)completion;   // iPad
- (void)dismissAnimated:(BOOL)animated;

@end

NS_ASSUME_NONNULL_END
