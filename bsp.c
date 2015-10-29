#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "bsp.h"

#define KEY_EXIT    27
#define KEY_PANIC   'p'
#define KEY_SICK    's'
#define KEY_OK      'o'
// Ack is generic for all messages
#define KEY_ACK     'a'
#define KEY_QUERY   'q'

char lastKeyPressed;
LogLevel currentLogLevel = LogLevelVerbose;

void handleCommunications(void)
{
    puts("(awaiting event)\n");
    lastKeyPressed = getch();

    if (lastKeyPressed == KEY_EXIT)
    {
        exit(0);
    }
}

int isSickButtonPressed(void)
{
    return (lastKeyPressed == KEY_SICK) ? 1 : 0;
}

int isPanicButtonPressed(void)
{
    return (lastKeyPressed == KEY_PANIC) ? 1 : 0;
}

int isOkButtonPressed(void)
{
    return (lastKeyPressed == KEY_OK) ? 1 : 0;
}

void sendSickMessage(void)
{
    puts("Sending SICK message\n");
    return;
}

int isSickAckReceived(void)
{
    return (lastKeyPressed == KEY_ACK) ? 1 : 0;
}

void sendPanicMessage(void)
{
    puts("Sending PANIC message\n");
    return;
}

int isPanicAckReceived(void)
{
    return (lastKeyPressed == KEY_ACK) ? 1 : 0;
}

int isQueryReceived(void)
{
    return (lastKeyPressed == KEY_QUERY) ? 1 : 0;
}

void sendQueryOkMessage(void)
{
    puts("Sending QUERY OK message\n");
    return;
}

int isQueryOkAckReceived(void)
{
    return (lastKeyPressed == KEY_ACK) ? 1 : 0;
}

void addLogEntry(const char *text, LogLevel loglevel)
{
    if (loglevel <= currentLogLevel)
    {
        puts(text);
    }
    return;
}

void addLogEntryWithState(const char *text, State currentState, LogLevel loglevel)
{
    if (loglevel <= currentLogLevel)
    {
        printf("%s (State: %s)\n", text, state2text(currentState));
    }
    return;
}

void assert(int conditionResult)
{
    if (conditionResult)
    {
        while(1)
        {
        }
    }
}
