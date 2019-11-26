/*!
 *  AirIdSCard.h
 *
 *  Copyright (c) 2014 Unicept GmbH. All rights reserved.
 *  Copyright (c) 2017, 2018, 2019 certgate GmbH. All rights reserved.
 *
 *
 */

#ifndef AIRID_SCARD_H
#define AIRID_SCARD_H

#include "pcsclite.h"

#ifndef UC_EXTERN
#ifdef __cplusplus
#define UC_EXTERN extern "C" __attribute__((visibility("default")))
#else
#define UC_EXTERN extern __attribute__((visibility("default")))
#endif
#endif

 
UC_EXTERN SCARD_IO_REQUEST const unicept_g_rgSCardT0Pci;
UC_EXTERN SCARD_IO_REQUEST const unicept_g_rgSCardT1Pci;
UC_EXTERN SCARD_IO_REQUEST const unicept_g_rgSCardRawPci;


#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @function unicept_SCardEstablishContext
 *
 * @abstract
 *     The unicept_SCardEstablishContext function establishes the resource manager context (the scope) within which database
 *     operations are performed.
 *
 * @discussion
 *     This is the first API call neccessary to be able to communicate with the Unicept(R) AirID(R) smart card reader.
 *     The context handle returned by unicept_SCardEstablishContext can be used by database query and management functions.
 *     To release an established resource manager context, use unicept_SCardReleaseContext.
 *
 *     Special note:
 *         the current implementation allows only ONE context to be established at a time.
 *
 * @param dwScope [in]
 *     Scope of the resource manager context. This parameter can be one of the following values:
 *     SCARD_SCOPE_USER   : Database operations are performed within the domain of the user.
 *     SCARD_SCOPE_SYSTEM : Database operations are performed within the domain of the system.
 *                          The calling application must have appropriate access permissions for any database actions.
 * @param pbPowerMan [in]
 *     Reserved for future use and must be NULL.
 *
 * @param pvReserved2 [in]
 *     Reserved for future use and must be NULL.
 *
 * @param phContext [out]
 *     A handle to the established resource manager context.
 *     This handle can then be supplied to other functions attempting to do work within this context.
 *
 * @returns
 *     If the function succeeds, the function returns SCARD_S_SUCCESS.
 *     If the function fails, it returns an error code.
 */
extern LONG unicept_SCardEstablishContext(
    DWORD dwScope,
    LPCVOID pbPowerMan,
    LPCVOID pvReserved2,
    LPSCARDCONTEXT phContext);

/*!
 * @function unicept_SCardIsValidContext
 *
 *
 * @discussion
 *     The unicept_SCardIsValidContext function determines whether a smart card context handle is valid.
 *
 * @discussion
 *     Call this function to determine whether a smart card context handle is still valid.
 *     After a smart card context handle has been set by SCardEstablishContext, it may become
 *     not valid if the resource manager service has been shut down.
 *
 * @param hContext [in]
 *     The context to be tested
 *
 * @returns
 *     If the given context is valid,   the function returns SCARD_S_SUCCESS.
 *     If the given context is invalid, the function returns SCARD_E_INVALID_HANDLE.
*/
extern LONG unicept_SCardIsValidContext(SCARDCONTEXT hContext);

/*!
 * @function unicept_SCardReleaseContext
 * @abstract
 *     The unicept_SCardReleaseContext function closes an established resource manager context, freeing any resources
 *     allocated under that context, including SCARDHANDLE objects and memory allocated using the
 *     SCARD_AUTOALLOCATE length designator.
 *
 * @discussion
 *     This function must be called for any SCARDCONTEXT previously aquired using the unicept_SCardEstablishContext,
 *     that is no longer needed.
 *
 * @param hContext [in]
 *     Handle that identifies the resource manager context. The resource manager context is set by a previous
 *     call to SCardEstablishContext.
 *
 * @returns
 *     If the function succeeds, the function returns SCARD_S_SUCCESS.
 *     If the function fails, it returns an error code.
 */
extern LONG unicept_SCardReleaseContext(SCARDCONTEXT hContext);

/*!
 * @function unicept_SCardListReaders
 *
 * @abstract
 *     The unicept_SCardListReaders function provides the list of readers within a set of named reader groups,
 *     eliminating duplicates.
 *
 * @discussion
 *     The caller supplies a list of reader groups, and receives the list of readers within the named groups.
 *     Unrecognized group names are ignored. This function only returns readers within the named groups that
 *     are currently attached to the system and available for use.
 *
 * @param hContext [in]
 *     Handle that identifies the resource manager context for the query. The resource manager context can be
 *     set by a previous call to SCardEstablishContext.
 *     If this parameter is set to NULL, the search for readers is not limited to any context.
 *
 * @param mszGroups [in]
 *     Names of the reader groups defined to the system, as a multi-string.
 *     Use a NULL value to list all readers in the system (that is, the SCard$AllReaders group).
 *     | Value                                                  | Meaning                                                                                                                                                          |
 *     |--------------------------------------------------------|:----------------------------------------------------------------------------------------------------------------------------------------------------------------:|
 *     | SCARD_ALL_READERS     TEXT("SCard$AllReaders\000")     | Group used when no group name is provided when listing readers. Returns a list of all readers, regardless of what group or groups the readers are in.            |
 *     | SCARD_DEFAULT_READERS TEXT("SCard$DefaultReaders\000") | Default group to which all readers are added when introduced into the system.                                                                                    |
 *     | SCARD_LOCAL_READERS   TEXT("SCard$LocalReaders\000")   | Unused legacy value. This is an internally managed group that cannot be modified by using any reader group APIs. It is intended to be used for enumeration only. |
 *     | SCARD_SYSTEM_READERS  TEXT("SCard$SystemReaders\000")  | Unused legacy value. This is an internally managed group that cannot be modified by using any reader group APIs. It is intended to be used for enumeration only. |
 *
 * @param mszReaders [out]
 *     Multi-string that lists the card readers within the supplied reader groups. If this value is NULL,
 *     SCardListReaders ignores the buffer length supplied in pcchReaders, writes the length of the buffer that
 *     would have been returned if this parameter had not been NULL to pcchReaders, and returns a success code.
 *     Note: This approche is unsafe, because the actual length of the multi-string might change over time.
 *
 * @param pcchReaders [out]
 *     Length of the mszReaders buffer in characters. This parameter receives the actual length of the
 *     multi-string structure, including all trailing null characters. If the buffer length is specified as
 *     SCARD_AUTOALLOCATE, then mszReaders is converted to a pointer to a byte pointer, and receives the
 *     address of a block of memory containing the multi-string structure. This block of memory must be
 *     deallocated with SCardFreeMemory. Using SCARD_AUTOALLOCATE is the preferred and secure way to go.
 *
 * @returns
 *     If the function succeeds, the function returns SCARD_S_SUCCESS.
 *     If the function fails, it returns an error code like
 *     SCARD_E_NO_READERS_AVAILABLE, SCARD_E_READER_UNAVAILABLE or any other SCARD error code.
 */
extern LONG unicept_SCardListReaders(
    SCARDCONTEXT hContext,
    LPCSTR mszGroups,
    LPSTR mszReaders,
    LPDWORD pcchReaders);

/*!
 * @function unicept_SCardFreeMemory
 *
 * @abstract
 *     The unicept_SCardFreeMemory function releases memory that has been returned from the resource manager
 *     using the SCARD_AUTOALLOCATE length designator.
 *
 * @param hContext [in]
 *     Handle that identifies the resource manager context for the query. The resource manager context can be
 *     set by a previous call to SCardEstablishContext.
 *
 * @param pvMem [in]
 *     Memory block to be released.
 *
 *
 * @returns
 *     If the function succeeds, the function returns SCARD_S_SUCCESS.
 *     If the function fails, it returns an error.
 */
extern LONG unicept_SCardFreeMemory(
    SCARDCONTEXT hContext,
    LPCVOID pvMem);

/*!
 * @function unicept_SCardGetStatusChange
 *
 * @abstract
 *     The SCardGetStatusChange function blocks execution until the current availability of the cards
 *     in a specific set of readers changes.
 *
 * @discussion
 *     The caller supplies a list of readers to be monitored by an SCARD_READERSTATE array and the maximum
 *     amount of time (in milliseconds) that it is willing to wait for an action to occur on one of the
 *     listed readers. Note that unicept_SCardGetStatusChange uses the user-supplied value in the dwCurrentState
 *     members of the rgReaderStates SCARD_READERSTATE array as the definition of the current state of the
 *     readers. The function returns when there is a change in availability, having filled in the
 *     dwEventState members of rgReaderStates appropriately.
 *
 * @param hContext [in]
 *     A handle that identifies the resource manager context. The resource manager context is set by a
 *     previous call to the SCardEstablishContext function.
 *
 * @param dwTimeout [in]
 *     The maximum amount of time, in milliseconds, to wait for an action. A value of zero causes the
 *     function to return immediately. A value of INFINITE causes this function never to time out.
 *
 * @param rgReaderStates [in, out]
 *     An array of SCARD_READERSTATE structures that specify the readers to watch, and that receives the result.
 *     To be notified of the arrival of a new smart card reader, set the szReader member of a SCARD_READERSTATE
 *     structure to "\\\\?PnP?\\Notification", and set all of the other members of that structure to zero.
 *     Important  Each member of each structure in this array must be initialized to zero and then set to
 *     specific values as necessary. If this is not done, the function will fail in situations that involve
 *     remote card readers.
 *
 * @param cReaders [in]
 *     The number of elements in the rgReaderStates array.
 *
 * @returns
 *     If the function succeeds, the function returns SCARD_S_SUCCESS.
 *     If the function fails, it returns an error code.
 */
extern LONG unicept_SCardGetStatusChange(
    SCARDCONTEXT hContext,
    DWORD dwTimeout,
    LPSCARD_READERSTATE rgReaderStates,
    DWORD cReaders);

/*!
 * @function unicept_SCardConnect
 *
 * @abstract
 *     The SCardConnect function establishes a connection (using a specific resource manager context) between
 *     the calling application and a smart card contained by a specific reader. If no card exists in the specified
 *     reader, an error is returned.
 *
 * @param hContext [in]
 *     A handle that identifies the resource manager context. The resource manager context is set by a previous
 *     call to SCardEstablishContext.
 *
 * @param szReader [in]
 *     The name of the reader that contains the target card.
 *
 * @param dwShareMode [in]
 *     A flag that indicates whether other applications may form connections to the card.
 *     | Value                 | Meaning                                                                                                                                             |
 *     |-----------------------|:---------------------------------------------------------------------------------------------------------------------------------------------------:|
 *     | SCARD_SHARE_SHARED    | This application is willing to share the card with other applications.                                                                              |
 *     | SCARD_SHARE_EXCLUSIVE | This application is not willing to share the card with other applications.                                                                          |
 *     | SCARD_SHARE_DIRECT    | This application is allocating the reader for its private use, and will be controlling it directly. No other applications are allowed access to it. |
 *
 * @param dwPreferredProtocols [in]
 *     A bitmask of acceptable protocols for the connection. Possible values may be combined with the OR operation.
 *
 *     | Value             | Meaning                                                                                                                                                                                                                                |
 *     |-------------------|:--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------:|
 *     | SCARD_PROTOCOL_T0 | T=0 is an acceptable protocol.                                                                                                                                                                                                         |
 *     | SCARD_PROTOCOL_T1 | T=1 is an acceptable protocol.                                                                                                                                                                                                         |
 *     | 0                 | This parameter may be zero only if dwShareMode is set to SCARD_SHARE_DIRECT. In this case, no protocol negotiation will be performed by the drivers until an IOCTL_SMARTCARD_SET_PROTOCOL control directive is sent with SCardControl. |
 *
 * @param phCard [out]
 *     A handle that identifies the connection to the smart card in the designated reader.
 *
 * @param pdwActiveProtocol [out]
 *     A flag that indicates the established active protocol.
 *
 *     | Value                    | Meaning                                                                                                                                  |
 *     |--------------------------|:----------------------------------------------------------------------------------------------------------------------------------------:|
 *     | SCARD_PROTOCOL_T0        | T=0 is an acceptable protocol.                                                                                                           |
 *     | SCARD_PROTOCOL_T1        | T=1 is an acceptable protocol.                                                                                                           |
 *     | SCARD_PROTOCOL_UNDEFINED | SCARD_SHARE_DIRECT has been specified, so that no protocol negotiation has occurred. It is possible that there is no card in the reader. |
 *
 * @returns
 *     If the function succeeds, the function returns SCARD_S_SUCCESS.
 *     If the function fails, it returns an error code.
 */
extern LONG unicept_SCardConnect(
    SCARDCONTEXT hContext,
    LPCSTR szReader,
    DWORD dwShareMode,
    DWORD dwPreferredProtocols,
    LPSCARDHANDLE phCard,
    LPDWORD pdwActiveProtocol);

/*!
 * @function unicept_SCardReconnect
 *
 * @abstract
 *     The SCardReconnect function reestablishes an existing connection between the calling application and
 *     a smart card. This function moves a card handle from direct access to general access, or acknowledges
 *     and clears an error condition that is preventing further access to the card.
 *
 * @param hCard [in]
 *     Reference value obtained from a previous call to SCardConnect.
 *
 * @param dwShareMode [in]
 *     A flag that indicates whether other applications may form connections to the card.
 *
 *     | Value                 | Meaning                                                                                                                                             |
 *     |-----------------------|:---------------------------------------------------------------------------------------------------------------------------------------------------:|
 *     | SCARD_SHARE_EXCLUSIVE | This application is not willing to share the card with other applications.                                                                          |
 *     | SCARD_SHARE_SHARED    | This application is     willing to share the card with other applications.                                                                          |
 *     | SCARD_SHARE_DIRECT    | This application is allocating the reader for its private use, and will be controlling it directly. No other applications are allowed access to it. |
 *
 * @param dwInitialization [in]
 *     Type of initialization that should be performed on the card.
 *
 *     | Value              | Meaning                                        |
 *     |--------------------|:----------------------------------------------:|
 *     | SCARD_LEAVE_CARD   | Do not do anything special on reconnect.       |
 *     | SCARD_RESET_CARD   | Reset the card (Warm Reset).                   |
 *     | SCARD_UNPOWER_CARD | Power down the card and reset it (Cold Reset). |
 *
 * @param dwPreferredProtocols [in]
 *     A bitmask of acceptable protocols for the connection. Possible values may be combined with the OR operation.
 *
 *     | Value             | Meaning                          |
 *     |-------------------|:--------------------------------:|
 *     | SCARD_PROTOCOL_T0 | T=0 is an acceptable protocol.   |
 *     | SCARD_PROTOCOL_T1 | T=1 is an acceptable protocol.   |
 *     | 0                 | This parameter may be zero only if dwShareMode is set to SCARD_SHARE_DIRECT. In this case, no protocol negotiation will be performed by the drivers until an IOCTL_SMARTCARD_SET_PROTOCOL control directive is sent with SCardControl. |
 *
 *
 * @param pdwActiveProtocol [out]
 *     A flag that indicates the established active protocol.
 *
 *     | Value                    | Meaning                          |
 *     |--------------------------|:--------------------------------:|
 *     | SCARD_PROTOCOL_T0        | T=0 is an acceptable protocol.   |
 *     | SCARD_PROTOCOL_T1        | T=1 is an acceptable protocol.   |
 *     | SCARD_PROTOCOL_UNDEFINED | SCARD_SHARE_DIRECT has been specified, so that no protocol negotiation has occurred. It is possible that there is no card in the reader. |
 *
 * @returns
 *     If the function succeeds, the function returns SCARD_S_SUCCESS.
 *     If the function fails, it returns an error code.
 */
extern LONG unicept_SCardReconnect(
    SCARDHANDLE hCard,
    DWORD dwShareMode,
    DWORD dwPreferredProtocols,
    DWORD dwInitialization,
    LPDWORD pdwActiveProtocol);

/*!
 * @function unicept_SCardDisconnect
 *
 * @abstract
 *     The SCardDisconnect function terminates a connection previously opened between the calling
 *     application and a smart card in the target reader.
 *
 * @param hCard [in]
 *     Reference value obtained from a previous call to SCardConnect.
 *
 * @param dwDisposition [in]
 *     Type of initialization that should be performed on the card.
 *
 *     | Value              | Meaning                                                              |
 *     |--------------------|:--------------------------------------------------------------------:|
 *     | SCARD_LEAVE_CARD   | Do not do anything special on reconnect.                             |
 *     | SCARD_RESET_CARD   | Reset the card (Warm Reset).                                         |
 *     | SCARD_UNPOWER_CARD | Power down the card and reset it (Cold Reset).                       |
 *     | SCARD_EJECT_CARD   | Eject the card (not supported in devices without an ejection servo). |
 *
 * @returns
 *     If the function succeeds, the function returns SCARD_S_SUCCESS.
 *     If the function fails, it returns an error code.
 */
extern LONG unicept_SCardDisconnect(
    SCARDHANDLE hCard,
    DWORD dwDisposition);

/*!
 * @function unicept_SCardControl
 *
 * @abstract
 *     The SCardControl function gives you direct control of the reader. You can call it any time after
 *     a successful call to SCardConnect and before a successful call to SCardDisconnect.
 *     The effect on the state of the reader depends on the control code.
 *
 * @param hCard [in]
 *     Reference value obtained from a previous call to SCardConnect.
 *
 * @param dwControlCode [in]
 *     Control code for the operation. This value identifies the specific operation to be performed.
 *
 * @param pbSendBuffer [in]
 *     Pointer to a buffer that contains the data required to perform the operation. This parameter
 *     can be NULL if the dwControlCode parameter specifies an operation that does not require input data.
 *
 * @param cbSendLength [in]
 *     Size, in bytes, of the buffer pointed to by lpInBuffer.
 *
 * @param pbRecvBuffer [out]
 *     Pointer to a buffer that receives the operation's output data. This parameter can be NULL if the
 *     dwControlCode parameter specifies an operation that does not produce output data.
 *
 * @param cbRecvLength [in]
 *     Size, in bytes, of the buffer pointed to by lpOutBuffer. 
 *
 * @param lpBytesReturned [out]
 *     Pointer to a DWORD that receives the size, in bytes, of the data stored into the buffer pointed
 *     to by lpOutBuffer.
 *
 * @returns
 *     If the function succeeds, the function returns SCARD_S_SUCCESS.
 *     If the function fails, it returns an error code.
 */
extern LONG unicept_SCardControl(
    SCARDHANDLE hCard,
    DWORD dwControlCode,
    LPCVOID pbSendBuffer,
    DWORD cbSendLength,
    LPVOID pbRecvBuffer,
    DWORD cbRecvLength,
    LPDWORD lpBytesReturned);

/*!
 * @function unicept_SCardStatus
 *
 * @abstract
 *     The SCardStatus function provides the current status of a smart card in a reader.
 *     You can call it any time after a successful call to SCardConnect and before a successful call
 *     to SCardDisconnect. It does not affect the state of the reader or reader driver.
 *
 * @param hCard [in]
 *     Reference value returned from SCardConnect.
 *
 * @param szReaderName [out]
 *     List of display names (multiple string) by which the currently connected reader is known.
 *
 * @param pcchReaderLen [in, out, optional]
 *     On input, supplies the length of the szReaderName buffer.
 *     On output, receives the actual length (in characters) of the reader name list, including the
 *     trailing NULL character. If this buffer length is specified as SCARD_AUTOALLOCATE, then
 *     mszReaderName is converted to a pointer to a byte pointer, and it receives the address of
 *     a block of memory that contains the multiple-string structure.
 *
 * @param pdwState [out, optional]
 *     Current state of the smart card in the reader. Upon success, it receives one of the following
 *     state indicators. 
 *
 *     | Value            | Meaning                                                                                        |
 *     |------------------|:----------------------------------------------------------------------------------------------:|
 *     | SCARD_ABSENT     | There is no card in the reader.                                                                |
 *     | SCARD_PRESENT    | There is a card in the reader, but it has not been moved into position for use.                |
 *     | SCARD_SWALLOWED  | There is a card in the reader in position for use. The card is not powered.                    |
 *     | SCARD_POWERED    | Power is being provided to the card, but the reader driver is unaware of the mode of the card. |
 *     | SCARD_NEGOTIABLE | The card has been reset and is awaiting PTS negotiation.                                       |
 *     | SCARD_SPECIFIC   | The card has been reset and specific communication protocols have been established.            |
 *
 * @param pdwProtocol [out, optional]
 *     Current protocol, if any. The returned value is meaningful only if the returned value of pdwState is SCARD_SPECIFICMODE.
 *
 *     | Value              | Meaning                                |
 *     |--------------------|:--------------------------------------:|
 *     | SCARD_PROTOCOL_RAW | The Raw Transfer protocol is in use.   |
 *     | SCARD_PROTOCOL_T0  | The ISO 7816/3 T=0 protocol is in use. |
 *     | SCARD_PROTOCOL_T1  | The ISO 7816/3 T=1 protocol is in use. |
 *
 * @param pbAtr [out]
 *     Pointer to a 32-byte buffer that receives the ATR string from the currently inserted card, if available.
 *
 * @param pcbAtrLen [in, out, optional]
 *     On input, supplies the length of the pbAtr buffer. On output, receives the number of bytes in the ATR string (32 bytes maximum).
 *     If this buffer length is specified as SCARD_AUTOALLOCATE, then pbAtr is converted to a pointer to a byte pointer, and it
 *     receives the address of a block of memory that contains the multiple-string structure.
 *
 * @returns
 *     If the function succeeds, the function returns SCARD_S_SUCCESS.
 *     If the function fails, it returns an error code.
 */
extern LONG unicept_SCardStatus(
    SCARDHANDLE hCard,
    LPSTR szReaderName,
    LPDWORD pcchReaderLen,
    LPDWORD pdwState,
    LPDWORD pdwProtocol,
    LPBYTE pbAtr,
    LPDWORD pcbAtrLen);

/*!
 * @function unicept_SCardTransmit
 *
 * @abstract
 *     The unicept_SCardTransmit function sends a service request to the smart card and expects to receive data back
 *     from the card.
 *
 * @param hCard [in]
 *     Reference value obtained from a previous call to SCardConnect.
 *
 * @param pioSendPci [in]
 *     A pointer to the protocol header structure for the instruction. This buffer is in the format of an
 *     SCARD_IO_REQUEST structure, followed by the specific protocol control information (PCI).
 *     For the T=0, T=1, and Raw protocols, the PCI structure is constant. The smart card subsystem supplies
 *     a global T=0, T=1, or Raw PCI structure, which you can reference by using the symbols
 *     SCARD_PCI_T0, SCARD_PCI_T1, and SCARD_PCI_RAW respectively.
 *
 * @param pbSendBuffer [in]
 *    A pointer to the actual data to be written to the card.
 *    For T=0, the data parameters are placed into the address pointed to by pbSendBuffer according to the following structure:
 * <pre>
 *    struct {
 *        &#9;BYTE bCla,
 *        &#9;BYTE bIns,
 *        &#9;BYTE bP1,
 *        &#9;BYTE bP2,
 *        &#9;BYTE bP3;
 *    } CmdBytes;
 * </pre>
 *    The data sent to the card should immediately follow the send buffer. In the special case where no data is sent to the card
 *    and no data is expected in return, bP3 is not sent.
 *
 *     | Member   | Meaning                                                                                      |
 *     |----------|:---------------------------------------------------------------------------------------------|
 *     | bCla     | The T=0 instruction class.                                                                   |
 *     | bIns     | An instruction code in the T=0 instruction class.                                            |
 *     | bP1, bP2 | Reference codes that complete the instruction code.                                          |
 *     | bP3      | The number of data bytes to be transmitted during the command, per ISO 7816-4, Section 8.2.1 |
 *
 *     Pointer to a buffer that contains the data required to perform the operation. This parameter
 *     can be NULL if the dwControlCode parameter specifies an operation that does not require input data.
 *
 * @param cbSendLength [in]
 *     The length, in bytes, of the pbSendBuffer parameter.
 *     For T=0, in the special case where no data is sent to the card and no data expected in return, this length
 *     must reflect that the bP3 member is not being sent; the length should be sizeof(CmdBytes) - sizeof(BYTE).
 *
 * @param pioRecvPci [out]
 *     Pointer to the protocol header structure for the instruction, followed by a buffer in which to receive any
 *     returned protocol control information (PCI) specific to the protocol in use. This parameter can be NULL if
 *     no PCI is returned.
 *
 * @param pbRecvBuffer [in]
 *     Pointer to any data returned from the card.
 *     For T=0, the data is immediately followed by the SW1 and SW2 status bytes. If no data is returned from the
 *     card, then this buffer will only contain the SW1 and SW2 status bytes.
 *
 * @param pcbRecvLength [in, out]
 *     Supplies the length, in bytes, of the pbRecvBuffer parameter and receives the actual number of bytes received
 *     from the smart card. This value cannot be SCARD_AUTOALLOCATE because SCardTransmit does not support SCARD_AUTOALLOCATE.
 *     For T=0, the receive buffer must be at least two bytes long to receive the SW1 and SW2 status bytes.
 *
 * @returns
 *     If the function succeeds, the function returns SCARD_S_SUCCESS.
 *     If the function fails, it returns an error code.
 */
extern LONG unicept_SCardTransmit(
    SCARDHANDLE hCard,
    const SCARD_IO_REQUEST *pioSendPci,
    LPCBYTE pbSendBuffer,
    DWORD cbSendLength,
    SCARD_IO_REQUEST *pioRecvPci,
    LPBYTE pbRecvBuffer,
    LPDWORD pcbRecvLength);

/*!
 * @function unicept_SCardBeginTransaction
 *
 * @abstract
 *     The SCardBeginTransaction function starts a transaction.
 *
 * @discussion
 *     The function waits for the completion of all other transactions before it begins. After the transaction starts,
 *     all other applications are blocked from accessing the smart card while the transaction is in progress.
 *     If a transaction is held on the card for more than five seconds with no operations happening on that card,
 *     then the card is reset. Calling any of the Smart Card and Reader Access Functions or Direct Card Access Functions
 *     on the card that is transacted results in the timer being reset to continue allowing the transaction to be used.
 *     The SCardBeginTransaction function is a smart card and reader access function.
 *
 * @param hCard [in]
 *     A reference value obtained from a previous call to SCardConnect.
 *
 * @returns
 *     If the function succeeds, the function returns SCARD_S_SUCCESS.
 *     If the function fails, it returns an error code.
 */
extern LONG unicept_SCardBeginTransaction(SCARDHANDLE hCard);

/*!
 * @function unicept_SCardEndTransaction
 *
 * @abstract
 *     The SCardEndTransaction function completes a previously declared transaction, allowing other applications
 *     to resume interactions with the card.
 *
 * @param hCard [in]
 *     Reference value obtained from a previous call to SCardConnect. This value would also have been used
 *     in an earlier call to SCardBeginTransaction.
 *
 * @param dwDisposition [in]
 *     Action to take on the card in the connected reader on close. 
 *
 *     | Value              | Meaning                                                              |
 *     |--------------------|:--------------------------------------------------------------------:|
 *     | SCARD_LEAVE_CARD   | Do not do anything special on reconnect.                             |
 *     | SCARD_RESET_CARD   | Reset the card (Warm Reset).                                         |
 *     | SCARD_UNPOWER_CARD | Power down the card and reset it (Cold Reset).                       |
 *     | SCARD_EJECT_CARD   | Eject the card (not supported in devices without an ejection servo). |
 *
 * @returns
 *     If the function succeeds, the function returns SCARD_S_SUCCESS.
 *     If the function fails, it returns an error code.
 */
extern LONG unicept_SCardEndTransaction(
    SCARDHANDLE hCard,
    DWORD dwDisposition);

#ifdef __cplusplus
}
#endif

#endif
