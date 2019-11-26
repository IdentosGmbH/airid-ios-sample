//
//  AIDDefines.m
//  AirIDDriver
//
//  Created by Wolfgang Sebastian Blaum on 08.10.14.
//  Updated by several people since then.
//  Copyright (c) 2014 Unicept GmbH. All rights reserved.
//  Copyright (c) 2017, 2018, 2019 certgate GmbH. All rights reserved.
//

#import <Foundation/Foundation.h>

// #ifdef AID_DEBUG
// #import "NSLogger.h"
// #endif

#ifndef AID_EXTERN
#ifdef __cplusplus
#define AID_EXTERN extern "C" __attribute__((visibility("default")))
#else
#define AID_EXTERN extern __attribute__((visibility("default")))
#endif
#endif


// Namespace pollution, now done wia AIDVersion
// #define VERSION @"0.16.1"

//! Project version number for AirIDDriver-iOS.
FOUNDATION_EXPORT double AirIDDriverVersionNumber;

//! Project version string for AirIDDriver-iOS.
FOUNDATION_EXPORT const unsigned char AirIDDriverVersionString[];

