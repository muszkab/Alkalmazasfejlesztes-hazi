#include "RobotStateHistory.h"
#include "RobotState.h"

RobotStateHistory::RobotStateHistory()
    : QObject(nullptr), currentState(nullptr)
{
}

void RobotStateHistory::Add(const RobotState& state)
{
    std::unique_ptr<RobotState> newState = std::make_unique<RobotState>();
    newState->CopyFrom(state);
    stateList.append(newState.get());

    currentState = (RobotState*)newState.get(); // This has to be done before moving ownership
    container.push_back(std::move(newState));

    // Összeállítjuk a grafikon számára az adatszerkezetet.
    // Ezek a listák QList<int> és QList<double> típusú listák, amiket direkt arra hozunk létre, hogy a QML elemek
    //  ezekből olvassák ki az adatokat, mivel erre a fenti container nem alkalmas. (Az std::vector
    //  nem támogatja a Qt metaobject rendszerét.)

    //Teljesen más megoldás, nekünk minden egyes lista elem kell a megjelenítéshez, nincs törlés, mindig egyet hozzá adunk
    graphVelocities.append(currentState->v());
    graphPositionX.append(currentState->x());
    graphPositionY.append(currentState->y());
    vectorOrient.append(currentState->orient());

    // Jelezzük a history változását.
    emit historyChanged();
}
