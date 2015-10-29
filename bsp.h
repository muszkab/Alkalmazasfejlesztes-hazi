/** Board Support Package - emulation for Windows environment */
#pragma once
#ifndef BSP_H
#define BSP_H
#include "state.h"

/** Loglevel definitions. */
typedef enum
{
    LogLevelNormal = 0,
    LogLevelVerbose = 1
} LogLevel;

/** Call this to wait for next event. */
void handleCommunications(void);

/** Returns nonzero if Sick button was pressed. */
int isSickButtonPressed(void);

/** Returns nonzero if Panic button was pressed. */
int isPanicButtonPressed(void);

/** Returns nonzero if OK button was pressed. */
int isOkButtonPressed(void);

/** Sends the sick message. */
void sendSickMessage(void);

/** Returns nonzero if the server acknowledged the sick message. */
int isSickAckReceived(void);

/** Sends the panic message. */
void sendPanicMessage(void);

/** Returns nonzero if the server acknowledged the panic message. */
int isPanicAckReceived(void);

/** Returns nonzero if the server asks whether the user is OK. */
int isQueryReceived(void);

/** Sends the OK message as a response to a query. */
void sendQueryOkMessage(void);

/** Returns nonzero if the server acknowledged the OK message. */
int isQueryOkAckReceived(void);

/** Adds a new entry to the system log. */
void addLogEntry(const char *text, LogLevel loglevel);

/** Adds a new entry and indicates the state in the system log. */
void addLogEntryWithState(const char *text, State currentState, LogLevel loglevel);

/** Default assert handler. If conditionResult is false, it should enter
 * infinite loop in debug environment or reset the controller
 * in non-debug mode. */
void assert(int conditionResult);

#endif // BSP_H

