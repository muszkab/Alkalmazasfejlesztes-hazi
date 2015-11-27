#include "szimulator.h"

Szimulator::Szimulator(int port)
    : communication(port), state()
{
    // Fogadni akarjuk a parancsokat
    connect(&communication, SIGNAL(dataReady(QDataStream&)), this, SLOT(dataReady(QDataStream&)));
    // Periodikusan futnia kell a szimulációnak
    connect(&timer, SIGNAL(timeout()), this, SLOT(tick()));
}

RobotState::koord Szimulator::SetKoordinata(float x, float y, qint8 o)
{
   RobotState::koord k;
   k.x=x;
   k.y=y;
   k.orient=o;
   return k;
}

RobotState::koord PositionCalculate (RobotState::koord prevPos, float v, float t)
{
    RobotState::koord newPos;
    newPos.x=prevPos.x+cos(prevPos.orient*15)*v*t;
    newPos.y=prevPos.y+sin(prevPos.orient*15)*v*t;
    newPos.orient=prevPos.orient;
    return newPos;
    /*FÖLÖSLEGES
    1. síknegyed: 0 és 6*15=90 fok között van az orientáció
     * x és y is nő
    if(prevPos.orient>=0 && prevPos.orient<6)
    {

    }
    2. síknegyed: 90 és 12*15=180 fok között van az orientáció
     * x csökken, y nő
    else if(prevPos.orient>=6 && prevPos.orient<12)
    {

    }
    3. síknegyed: 180 és 18*15=270 fok között van az orientáció
     * x és y is csökken
    else if(prevPos.orient>=12 && prevPos.orient<18)
    {

    }
    4. síknegyed: 270 és 23*15=360 fok között van az orientáció
     * x nő, y csökken
    else if(prevPos.orient>=18 && prevPos.orient<24)
    {

    }
    */

}

void Szimulator::start(float intervalSec)
{

    dt = intervalSec;
    state.setStatus(RobotState::Status::Default);
    state.setTimestamp(0);
    state.setPos(SetKoordinata(0,0,0));
    state.setV(0.0F);
    state.setA(0.0F);
    state.setLight(0);
    timer.start((long)(intervalSec*1000.0F));
}

void Szimulator::tick()
{
    // Fizikai szimuláció
    state.setTimestamp(state.timestamp() + dt);
    state.setPos(PositionCalculate(state.pos(),state.v(),dt));
    state.setV(state.v() + state.a()*dt);

    if (state.v()<-10.0)
    {
        state.setV( -10.0F );
    }
    if (state.v()>10.0)
    {
        state.setV( 10.0F );
    }

    state.setLight( state.v()<=0.1F ? 1.0F : 0.0F );

    // Magasabb szintű funkciók
    switch(state.status())
    {
    case RobotState::Status::Default:
        break;
    case RobotState::Status::Reset:
        qDebug() << "Szimulator: Reset";
        state.setStatus(RobotState::Status::Default);
        state.setPos(SetKoordinata(0,0,0));
        state.setV(0.0F);
        state.setA(0.0F);
        state.setLight(0);
        break;
    case RobotState::Status::Stopping:
        if (state.v() > 1.5F)
        {
            qDebug() << "Szimulator: Stop parancs, gyors lassítás";
            state.setA(-1.0F);
        }
        else if (state.v() > 0.1F)
        {
            qDebug() << "Szimulator: Stop parancs, lassú lassítás";
            state.setA(-0.05F);
        }
        else if (state.v() < -1.5F)
        {
            qDebug() << "Szimulator: Stop parancs, gyorsítás előre";
            state.setA(1.0F);
        }
        else if (state.v() < -0.1F)
        {
            qDebug() << "Szimulator: Stop parancs, lassú gyorsítás előre";
            state.setA(0.05F);
        }
        else
        {
            // Majdnem megállt
            qDebug() << "Szimulator: Megállt.";
            state.setStatus(RobotState::Status::Default);
            state.setA(0.0F);
        }
        break;
    case RobotState::Status::Accelerate:
        // Megjegyzés: Robotproxyban át kell írni!!
        //Ha max sebességgen van, nem gyorsítunk.
        if(state.v()>10.0)
        {
            state.setStatus(RobotState::Status::Default);
            state.setA(0.0F);
        }
        else
        {
            qDebug() << "Szimulator: Gyorítás.";
            state.setStatus(RobotState::Status::Accelerate);
            state.setA(1.0F);
        }
        break;
    case RobotState::Status::Right:
        qDebug() << "Szimulator: Right parancs, 15 fok jobbra";
        if(state.pos().orient==0)
        {
            state.setPos(SetKoordinata(state.pos().x, state.pos().y, 23));
        }
        else
        {
            state.setPos(SetKoordinata(state.pos().x, state.pos().y, state.pos().orient-1));
        }

        break;
    case RobotState::Status::Left:
        qDebug() << "Szimulator: Left parancs, 15 fok balra";
        if(state.pos().orient==23)
        {
            state.setPos(SetKoordinata(state.pos().x, state.pos().y, 0));
        }
        else
        {
            state.setPos(SetKoordinata(state.pos().x, state.pos().y, state.pos().orient+1));
        }
        break;
    default:
        Q_UNREACHABLE();
    }

    qDebug() << "Szimulator: tick (" << state.timestamp()
             << "): állapot=" << state.getStatusName()
             << ", x=" << state.pos().x
             << ", y=" << state.pos().y
             << ", v=" << state.v()
             << ", a=" << state.a()
             << ", lámpa:" << state.light();

    // Állapot küldése
    if (communication.isConnected())
    {
        communication.send(state);
    }
}

void Szimulator::dataReady(QDataStream &inputStream)
{
    RobotState receivedState;
    receivedState.ReadFrom(inputStream);

    switch(receivedState.status())
    {
    case RobotState::Status::Default:
        qDebug() << "Szimulator: OK parancs. Igen, minden OK, köszönöm!";
        break;
    case RobotState::Status::Reset:
        qDebug() << "Szimulator: Reset parancs.";
        state.setStatus(RobotState::Status::Reset);
        break;
    case RobotState::Status::Stopping:
        qDebug() << "Szimulator: Stop parancs.";
        state.setStatus(RobotState::Status::Stopping);
        break;
    case RobotState::Status::Accelerate:
        qDebug() << "Szimulator: Gyorsítási parancs.";
        state.setStatus(RobotState::Status::Default);
        state.setA(receivedState.a());
        break;
    default:
        Q_UNREACHABLE();
    }
}

