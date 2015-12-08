#include <QtMath>
#include "szimulator.h"

#define alfa 15

Szimulator::Szimulator(int port)
    : communication(port), state()
{
    // Fogadni akarjuk a parancsokat
    connect(&communication, SIGNAL(dataReady(QDataStream&)), this, SLOT(dataReady(QDataStream&)));
    // Periodikusan futnia kell a szimulációnak
    connect(&timer, SIGNAL(timeout()), this, SLOT(tick()));
}

RobotState::koord Szimulator::SetKoordinata(float x, float y, qint16 o, qint16 t)
{
   RobotState::koord k;
   k.x=x;
   k.y=y;
   k.orient=o;
   k.turn=t;
   return k;
}

RobotState::koord Szimulator::PositionCalculate (RobotState::koord prevPos, float v, float t)
{
    RobotState::koord newPos;
    if(v>0.1F) newPos.orient=prevPos.orient-prevPos.turn;
    else newPos.orient=prevPos.orient;
    newPos.x=prevPos.x+cos((double)prevPos.orient/360*2*M_PI)*v*t;
    newPos.y=prevPos.y+sin((double)prevPos.orient/360*2*M_PI)*v*t;
    newPos.turn=prevPos.turn;
    return newPos;
}

void Szimulator::start(float intervalSec)
{

    dt = intervalSec;
    state.setStatus(RobotState::Status::Default);
    state.setTimestamp(0);
    state.setPos(SetKoordinata(0,0,0,0));
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
        state.setA(0.0F); //Semmiképp se legyen gyorsulás az alap állapotban, csak tartsa a sebbességet
        break;
    case RobotState::Status::Reset:
        qDebug() << "Szimulator: Reset";
        state.setStatus(RobotState::Status::Default);
        state.setPos(SetKoordinata(0,0,0,0));
        state.setV(0.0F);
        state.setA(0.0F);
        state.setLight(0);
        break;
    case RobotState::Status::Stopping:
        if (state.v() > 0)
        {
            qDebug() << "Szimulator: Stop parancs, gyors lassítás";
            state.setA(-1.0F);
        }
        else
        {
            qDebug() << "Szimulator: Megállt.";
            state.setStatus(RobotState::Status::Default);
            state.setA(0.0F);
            state.setV(0.0F);
        }
        break;
    case RobotState::Status::Accelerate:
        // Megjegyzés: Robotproxyban át kell írni!!
        //Ha max sebességgen van, nem gyorsítunk.
        if(state.v()>=10.0)
        {
            state.setStatus(RobotState::Status::Default);
            state.setA(0.0F);
        }
        else
        {
            qDebug() << "Szimulator: Gyorsítás.";
            state.setStatus(RobotState::Status::Accelerate);
            state.setA(1.0F);
        }
        break;
    case RobotState::Status::SelfTest:

        if(true){
            qDebug() << "Szimulator: Önteszt";
        }
        break;

        /**  Nem biztos h kell bele
         *
    case RobotState::Status::Right:
        qDebug() << "Szimulator: Right parancs, alfa fok jobbra";
        if(state.pos().orient==0)
        {
            state.setPos(SetKoordinata(state.pos().x, state.pos().y, 359, state.pos().turn));
        }
        else
        {
            state.setPos(SetKoordinata(state.pos().x, state.pos().y, state.pos().orient-state.pos().turn, state.pos().turn));
        }

        break;
    case RobotState::Status::Left:
        qDebug() << "Szimulator: Left parancs, alfa fok balra";
        if(state.pos().orient==359)
        {
            state.setPos(SetKoordinata(state.pos().x, state.pos().y, 0,state.pos().turn));
        }
        else
        {
            state.setPos(SetKoordinata(state.pos().x, state.pos().y, state.pos().orient+alfa,state.pos().turn));
        }
        break;
        */
    default:
        Q_UNREACHABLE();
    }

    qDebug() << "Szimulator: tick (" << state.timestamp()
             << "): állapot=" << state.getStatusName()
             << ", turn=" << state.turn()
             << ", x=" << state.x()
             << ", y=" << state.y()
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
        state.setStatus(RobotState::Status::Accelerate);
        break;
    case RobotState::Status::Turn:
        qDebug() << "Szimulator: Kanyarodás parancs.";
        state.setTurn(receivedState.turn());
    case RobotState::Status::SelfTest:
        qDebug() << "Szimulator: Önteszt parancs.";
        state.setStatus(RobotState::Status::SelfTest);
    break;
    default:
        Q_UNREACHABLE();
    }
}

