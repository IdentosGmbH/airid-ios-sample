/*
 * Copyright (c) 2018 Identos GmbH
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

#import "PBSmartcardUtilities.h"

@implementation PBSmartcardUtilities

+ (NSString*)toHexString:(NSData*)data {
    const unsigned char* bytes = data.bytes;
    NSUInteger length = data.length;
    NSMutableString* hexString = [NSMutableString stringWithCapacity:(length * 2)];
    
    for (int i = 0; i < length; i += 1) {
        [hexString appendFormat:@"%02X ", (unsigned int) bytes[i]];
    }
    
    return [hexString stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
}

+ (NSData*)fromHexString:(NSString*)hexString {
    NSString* strippedString = [hexString stringByReplacingOccurrencesOfString:@" " withString:@""];
    const char* chars = [strippedString UTF8String];
    NSUInteger length = strippedString.length;
    
    NSMutableData* data = [NSMutableData dataWithCapacity:length / 2];
    char byteChars[3] = {'\0','\0','\0'};
    
    int i = 0;
    
    while (i < length) {
        byteChars[0] = chars[i++];
        byteChars[1] = chars[i++];
        unsigned long wholeByte = strtoul(byteChars, NULL, 16);
        [data appendBytes:&wholeByte length:1];
    }
    
    return data;
}

#if 0
+ (NSString*)errorMessageFrom:(int)errorCode {
    switch (errorCode) {
        case PBSmartcardStatusSuccess:return @"PBSmartcardStatusSuccess";
        case PBSmartcardStatusInvalidParameter:return @"PBSmartcardStatusInvalidParameter";
        case PBSmartcardStatusSharingViolation:return @"PBSmartcardStatusSharingViolation";
        case PBSmartcardStatusNoSmartcard:return @"PBSmartcardStatusNoSmartcard";
        case PBSmartcardStatusProtocolMismatch:return @"PBSmartcardStatusProtocolMismatch";
        case PBSmartcardStatusNotReady:return @"PBSmartcardStatusNotReady";
        case PBSmartcardStatusInvalidValue:return @"PBSmartcardStatusInvalidValue";
        case PBSmartcardStatusReaderUnavailable:return @"PBSmartcardStatusReaderUnavailable";
        case PBSmartcardStatusUnexpected:return @"PBSmartcardStatusUnexpected";
        case PBSmartcardStatusUnsupportedCard:return @"PBSmartcardStatusUnsupportedCard";
        case PBSmartcardStatusUnresponsiveCard:return @"PBSmartcardStatusUnresponsiveCard";
        case PBSmartcardStatusUnpoweredCard:return @"PBSmartcardStatusUnpoweredCard";
        case PBSmartcardStatusResetCard:return @"PBSmartcardStatusResetCard";
        case PBSmartcardStatusRemovedCard:  return @"PBSmartcardStatusRemovedCard";
        case PBSmartcardStatusProtocolNotIncluded:return @"PBSmartcardStatusProtocolNotIncluded";
        case PBSmartcardStatusNotSupported:return @"PBSmartcardStatusNotSupported";
        default: return [NSString stringWithFormat:@"0x%08x", errorCode];
    }
}
#endif

+ (unsigned short)statusBytesFrom:(NSData*)data {
    if (data.length < 2) {
        return -1;
    }
    
    const unsigned char* bytes = [data bytes];

    return OSReadBigInt16(bytes, data.length - 2);
}

@end
