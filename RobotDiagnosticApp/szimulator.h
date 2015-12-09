#ifndef SZIMULATOR_H
#define SZIMULATOR_H

#include <QObject>
#include <QTimer>
#include "Communication/CommunicationTcpSocketServer.h"
#include "RobotState.h"

/**
 * @brief A robot szimulátor.
 *
 * Van egy belső, RobotState típusú állapota, melyet egy QTimer segítségével periodikusan frissít.
 * Létrehoz egy CommunicationTcpSocketServer objektumot a kommunikációhoz, amihez lehet csatlakozni.
 * Minden szimulációs lépés után elküldi az állapotát.
 * Ha egy RobotState objektumot kap, azt parancsként értelmezi.
 */
class Szimulator : public QObject
{
    Q_OBJECT

public:
    /** Konstruktor.
     * @param port  A port, amin a létrehozott szerver hallgatózik.
    */
    explicit Szimulator(int port);
    ~Szimulator() = default;

    /** Elindítja a szimulátort.
     * @param intervalSec   A szimulátor periódusideje.
     */
    void start(float intervalSec);

    /**
     * @brief A pozíció beállításához szükséges függvény.
     * @param x X koordinátát állítja be.
     * @param y Y koordinátát állítja be.
     * @param o Orientációt adhatjuk meg.
     * @param t Kanyarodás mértéke.
     */
    RobotState::koord SetKoordinata(float x, float y, qint16 o, qint16 t);

    /**
     * @brief Pozíciót (x,y) számítja ki minden egyes tick-nél.
     * @param prevPos Előző állapot pozíció adatai.
     * @param v Sebesség
     * @param t Kanyarosád mértéke fokban
     * @return
     */
    RobotState::koord PositionCalculate (RobotState::koord prevPos, float v, float t);

private:
    /** Belső szerver a kommunikációhoz. */
    CommunicationTcpSocketServer communication;

    /** Időzítő a tick() metódus periodikus hívására. */
    QTimer timer;

    /** A periódus idő (másodpercben). */
    float dt;

    /** A szimulátor pillanatnyi állapota. */
    RobotState state;

private slots:
    /** A timer hívja meg, meghatározza a robot
     * állapotát a következő időpillanatban. */
    void tick();

    /** Új üzenet fogadását jelzi. */
    void dataReady(QDataStream& inputStream);
};

#endif // SZIMULATOR_H
