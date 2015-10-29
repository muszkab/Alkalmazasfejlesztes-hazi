#include "state.h"

const char *stateStrings[] =
    { "Init", "Normal", "SendSick", "AwaitSickAck", "SendPanic", "AwaitPanicAck",
    "AwaitQueryOk", "SendQueryOk", "AwaitQueryOkAck" };

const char *state2text(State currentState)
{
    return stateStrings[(int)currentState];
}
