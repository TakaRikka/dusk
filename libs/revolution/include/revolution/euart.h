#ifndef _REVOLUTION_EUART_H_
#define _REVOLUTION_EUART_H_

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int UARTError;

typedef enum {
	kBaudHWSet = -1,
	kBaud300 = 300,
	kBaud600 = 600,
	kBaud1200 = 1200,
	kBaud1800 = 1800,
	kBaud2000 = 2000,
	kBaud2400 = 2400,
	kBaud3600 = 3600,
	kBaud4800 = 4800,
	kBaud7200 = 7200,
	kBaud9600 = 9600,
	kBaud19200 = 19200,
	kBaud38400 = 38400,
	kBaud57600 = 57600,
	kBaud115200 = 115200,
	kBaud230400 = 230400
} UARTBaudRate;

enum {
	kUARTDataError = -1,
	kUARTNoError = 0,
	kUARTUnknownBaudRate,
	kUARTConfigurationError,
	kUARTBufferOverflow,
	kUARTNoData
};

typedef enum {
	EUART_ERROR_NONE = 0,
	EUART_ERROR_UNINITIALIZED,
	EUART_ERROR_CANNOT_USE,
	EUART_ERROR_CHANNEL_FULL,
	EUART_ERROR_NOT_FIND,
	EUART_ERROR_INTERNAL,
	EUART_ERROR_INVALID_PARAMETER,
	EUART_ERROR_INVALID_HANDLE,
	EUART_ERROR_COM_OPEN,
	EUART_ERROR_COMM
} EUARTError;


UARTError InitializeUART(UARTBaudRate);
UARTError WriteUARTN(const void *, unsigned long);

#ifdef __cplusplus
}
#endif

#endif
