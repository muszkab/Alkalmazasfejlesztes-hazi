#include <QtMath>
#include "szimulator.h"
#include <QMessageBox>

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
    //Ha áll a robot, nem változik az orientáció. Ha öntesztelő állapotban, nem lehet kanyarodni.
    if(v==0 || state.light()) newPos.orient=prevPos.orient;
    //Orientációhoz hozzáadjuk az aktuális turn értéket (negatív előjellel), csak pozitív és 360-nál kisebb értéke lehet.
    else newPos.orient=(prevPos.orient-prevPos.turn+360)%360;
    //X koordináta számolása az orientáció, sebesség alapján (koszinusz)
    newPos.x=prevPos.x+cos((double)prevPos.orient/360*2*M_PI)*v*t;
    //Y koordináta számolása az orientáció, sebesség alapján (szinusz)
    newPos.y=prevPos.y+sin((double)prevPos.orient/360*2*M_PI)*v*t;
    //A kanyarodás (turn) mértékét kívülről kapja a Robotproxy felől,a csúszka állásának megfelelően.
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

    if (state.v()>10.0) //Maximum 10 m/s-val megy a robot
    {
        state.setV( 10.0F );
    }

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
        state.setLight(0); //önteszt funkció végleges befejezése, felirat eltüntetése
        if (state.v() > 0)
        {
            qDebug() << "Szimulator: Stop parancs, lassítás";
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
        state.setLight(0); //önteszt funkció végleges befejezése, felirat eltüntetése
        //Ha max sebességgen van, nem gyorsítunk, alap állapotba lépünk.
        if(state.v()>=10.0)
        {
            state.setStatus(RobotState::Status::Default);
            state.setA(0.0F);
        }
        else
        {
            qDebug() << "Szimulator: Gyorsítás.";
            state.setA(1.0F);
        }
        break;
    case RobotState::Status::SelfTest:
        //light változó alapján megy végig az önteszt fázisain
        switch(state.light())
        {
        case 1:
            //reseteljük a paramétereket
            qDebug() << "Szimulator: Öntesztelés indul.";
            state.setPos(SetKoordinata(0,0,0,0));
            state.setV(0.0F);
            state.setA(0.0F);
            state.setLight(2);
            break;
        case 2:
            //kanyarodás teszt
            if(state.turn()<20)
                state.setTurn(state.turn()+5);
            else
            {
                if(state.turn()<=20)
                {
                    qDebug() << "Szimulator: Jobbra kanyarodás működik.";
                    state.setLight(3);
                }
                else state.setLight(-1);
            }
            break;
        case 3:
            //kanyarodás teszt
            if(state.turn()>-20)
                state.setTurn(state.turn()-5);
            else
            {
                if(state.turn()>=-20)
                {
                    qDebug() << "Szimulator: Balra kanyarodás működik.";
                    state.setLight(4);
                }
                else state.setLight(-1);
            }
            break;
        case 4:
            //kanyarodás teszt
            if(state.turn()<0)
                state.setTurn(state.turn()+5);
            else
            {
                if(state.turn()>=0)
                {
                    state.setTurn(0);
                    qDebug() << "Szimulator: Kormány alaphelyzetbe";
                    state.setLight(5);
                }
                else state.setLight(-1);
            }
            break;
        case 5:
            //gyorsítás teszt
            if(state.v()<5){
                state.setA(1.0F);
            }
            else
            {
                if(state.v()>=5)
                {
                    state.setA(0.0F);
                    qDebug() << "Szimulator: Gyorsítás működik.";
                    state.setLight(6);
                }
                else state.setLight(-1);
            }
            break;
        case 6:
            //lassítás teszt
            if(state.v()>0){
                state.setA(-1.0F);
            }
            else
            {
                if(state.v()<=0)
                {
                    state.setA(0.0F);
                    qDebug() << "Szimulator: Lassítás működik.";
                    state.setLight(7);
                }
                else state.setLight(-1);
            }
            break;
        case 7:
            //megállás
            state.setA(0.0F);
            state.setV(0.0F);
            state.setLight(8);
            break;
        case 8:
            //alaphelyzetbe állítás
            if(state.v()==0 && state.a()==0)
            {
                QMessageBox msgBox;
                msgBox.setText("Sikeres önteszt!");
                msgBox.exec();
                state.setStatus(RobotState::Status::Default);
                qDebug() << "Szimulator: Sikeres önteszt!";
            }
            else state.setLight(-1);
            break;
        default:
            qDebug() << "Szimulator: Sikertelen önteszt, reset szükséges.";
        }
        break;
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
             << ", Önteszt állapot:" << state.light();

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
        if(state.light()==0 || state.light()==8)
        {
            qDebug() << "Szimulator: Stop parancs.";
            state.setStatus(RobotState::Status::Stopping);
        }
        else
            qDebug() << "Szimulator: Önteszt folyamatban, a parancs nem elérhető.";
        break;
    case RobotState::Status::Accelerate:        
        if(state.light()==0 || state.light()==8)
        {
            qDebug() << "Szimulator: Gyorsítási parancs.";
            state.setStatus(RobotState::Status::Accelerate);
        }
        else
            qDebug() << "Szimulator: Önteszt folyamatban, a parancs nem elérhető.";
        break;
    case RobotState::Status::Turn:
        if(state.light()==0 || state.light()==8)
        {
            qDebug() << "Szimulator: Kanyarodás parancs.";
            state.setTurn(receivedState.turn());
        }
        else
            qDebug() << "Szimulator: Önteszt folyamatban, a parancs nem elérhető.";
        break;
    case RobotState::Status::SelfTest:
        if(state.light()==0|| state.light()==8)
        {
            qDebug() << "Szimulator: Önteszt parancs.";
            state.setStatus(RobotState::Status::SelfTest);
            state.setLight(1);
        }
        else
            qDebug() << "Szimulator: Önteszt folyamatban, a parancs nem elérhető.";
        break;
    default:
        Q_UNREACHABLE();
    }
}

