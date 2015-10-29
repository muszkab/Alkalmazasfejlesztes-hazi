#include "bsp.h"

int main(void)
{
    State currentState = Normal;
    State previousState = Init;
    addLogEntry("System initialized", LogLevelVerbose);
    while(1)
    {
        handleCommunications();
        switch(currentState)
        {
        case Init:
            break;
        case Normal:
            if (isSickButtonPressed())
            {
                currentState = SendSick;
            }
            else if (isPanicButtonPressed())
            {
                currentState = SendPanic;
            }
            else if (isQueryReceived())
            {
                currentState = AwaitQueryOk;
            }
            break;
        case SendSick:
            sendSickMessage();
            currentState = AwaitSickAck;
            break;
        case AwaitSickAck:
            if (isSickAckReceived())
            {
                currentState = Normal;
            }
            else if (isPanicButtonPressed())
            {
                addLogEntry("Panic button pressed while waiting for sick acknowledge", LogLevelVerbose);
                currentState = SendPanic;
            }
            break;
        case SendPanic:
            sendPanicMessage();
            currentState = AwaitPanicAck;
            break;
        case AwaitPanicAck:
            if (isPanicAckReceived())
            {
                currentState = Normal;
            }
            break;
        case AwaitQueryOk:
            if (isOkButtonPressed())
            {
                currentState = SendQueryOk;
            }
            else if (isSickButtonPressed())
            {
                addLogEntry("Sick button pressed while waiting for OK", LogLevelVerbose);
                currentState = SendSick;
            }
            else if (isPanicButtonPressed())
            {
                addLogEntry("Panic button pressed while waiting for OK", LogLevelVerbose);
                currentState = SendPanic;
            }
            break;
        case SendQueryOk:
            sendQueryOkMessage();
            currentState = AwaitQueryOkAck;
            break;
        case AwaitQueryOkAck:
            if (isQueryOkAckReceived())
            {
                currentState = Normal;
            }
            else if (isSickButtonPressed())
            {
                addLogEntry("Sick button pressed while waiting for OK acknowledgement", LogLevelVerbose);
                currentState = SendSick;
            }
            else if (isPanicButtonPressed())
            {
                addLogEntry("Panic button pressed while waiting for OK acknowledgement", LogLevelVerbose);
                currentState = SendPanic;
            }
            break;
        default:
            addLogEntry("Unknown state", LogLevelNormal);
            assert(0);
            break;
        }
        if (currentState != previousState)
        {
            addLogEntryWithState("State changed", currentState, LogLevelNormal);
            previousState = currentState;
        }
    }
}
