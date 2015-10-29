/** Finite state machine state definitions. */
#pragma once
#ifndef STATE_H
#define STATE_H

typedef enum
{
    Init = 0,
    Normal,
    SendSick,
    AwaitSickAck,
    SendPanic,
    AwaitPanicAck,
    AwaitQueryOk,
    SendQueryOk,
    AwaitQueryOkAck
} State;

const char *state2text(State currentState);

#endif // STATE_H
