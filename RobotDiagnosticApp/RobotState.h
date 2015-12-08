#pragma once
#ifndef ROBOTSTATE_H
#define ROBOTSTATE_H
#include <QDataStream>
#include <QString>

/**
 * @brief A robot teljes állapotleírása le egy adott időpillanatban.
 *
 * A kommunikáció során ilyen objektumokat küldünk át a robot és a kliens között.
 * A robottól érkezve az aktuális állapotot képviseli, míg az ellenkező irányba küldve
 *  parancsként a kívánt állapotot írja le.
 *
 * A history ilyen objektumok sorozata.
 */
class RobotState : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief A robot állapota
     */
    enum class Status
    {
        /** Alapállapot. A robot tartja az aktuális gyorsulást. */
        Default = 0,
        /** Reset. Parancsként használjuk. */
        Reset = 1,
        /** Stop parancs. Ez az állapot akkor is érvényes marad, ha megállt a robot. */
        Stopping = 2,
        /** Gyorsítási parancs a robotnak. A gyorsítás mértékét a robot
         * átveszi a RobotState::a tulajdonságból. */
        Accelerate = 3,
        /** Kanyarodás parancs. A fordulás mértékét kívülről kapja. */
        Turn=4,
        /** Öntesztelő állapot */
        SelfTest=5,
        /** Jobbra kanyarodási parancs */
        Right = 6,
        /** Balra kanyarodási parancs */
        Left = 7
    };
    /** Koordiáták és orientáció a pozició tárolásához */
    typedef struct koord{
        /** X koordináta */
        float x;
        /** Y koordináta */
        float y;
        /** Orientáció, 0-tól 2*PI-ig lehet értéke.
         * Sebesség irányát is ez írja le.*/
        qint16 orient;
        /** Fordulás mértékét jellemzi */
        qint16 turn;
    }koord;

    /**
     * @brief Konstruktor
     */
    RobotState();

    /**
     * @brief Konstruktor adott értékekkel.
     * @param status    Robot állapot
     * @param timestamp Időbélyeg
     * @param pos Pozíció
     * @param v Sebesség
     * @param a Gyorsulás
     * @param light Robot lámpájának állapota
     */
    RobotState(Status status, qint64 timestamp,
       koord pos, float v, float a, qint8 light);

    ~RobotState() = default;

    // A NOTIFY signalokat nem implementáljuk, mert nincs rájuk szükség.

    /** Állapot (vagy parancs) */
    Q_PROPERTY(Status status READ status WRITE setStatus MEMBER _status NOTIFY statusChanged)
    Status status() const { return _status; }
    void setStatus(const Status status) { _status = status; }

    /** Időbélyeg (ms) */
    Q_PROPERTY(float timestamp READ timestamp WRITE setTimestamp MEMBER _timestamp NOTIFY timestampChanged)
    float timestamp() const { return _timestamp; }
    void setTimestamp(const float timestamp) { _timestamp = timestamp; }

    /** Pozíció (méter) */
    Q_PROPERTY(koord pos READ pos WRITE setPos MEMBER _pos NOTIFY posChanged)
    koord pos() const { return _pos; }
    void setPos(koord pos) { _pos = pos; }

    /** X koordináta (méter) */
    Q_PROPERTY(float x READ x WRITE setX MEMBER _pos NOTIFY xChanged)
    float x() const { return _pos.x; }
    void setX(float x) { _pos.x = x; }

    /** Y koordináta (méter) */
    Q_PROPERTY(float y READ y WRITE setY MEMBER _pos NOTIFY yChanged)
    float y() const { return _pos.y; }
    void setY(float y) { _pos.y = y; }

    /** Orientáció (fok) */
    Q_PROPERTY(qint16 orient READ orient WRITE setOrient MEMBER _pos NOTIFY orientChanged)
    qint16 orient() const { return _pos.orient; }
    void setOrient(qint16 orient) { _pos.orient = orient; }

    /** Kanyarodás (fok: orientáció változás egy tick alatt) */
    Q_PROPERTY(qint16 turn READ turn WRITE setTurn MEMBER _pos NOTIFY turnChanged)
    qint16 turn() const { return _pos.turn; }
    void setTurn(qint16 turn) { _pos.turn = turn; }

    /** Sebesség (m/s) */
    Q_PROPERTY(float v READ v WRITE setV MEMBER _v NOTIFY vChanged)
    float v() const { return _v; }
    void setV(float v) { _v = v; }

    /** Gyorsulás (m/s2) */
    Q_PROPERTY(float a READ a WRITE setA MEMBER _a NOTIFY aChanged)
    float a() const { return _a; }
    void setA(float a) { _a = a; }

    /** A robot lámpájának állapota. */
    Q_PROPERTY(bool light READ light WRITE setLight MEMBER _light NOTIFY lightChanged)
    float light() const { return _light; }
    void setLight(float light) { _light = light; }

    /** Az aktuális állapot QStringként. */
    // In QML, it will be accessible as model.statusName
    Q_PROPERTY(QString statusName READ getStatusName NOTIFY statusChanged)

    /** Sorosítja az objektumot a megadott streambe. */
    void WriteTo(QDataStream& stream) const;

    /** Beolvassa az objektumot a streamből. */
    void ReadFrom(QDataStream& stream);

    /** Másolat készítés. */
    // Erre azért van szükség, mert a QObject-ek másolására speciális szabályok vonatkoznak.
    void CopyFrom(const RobotState& other);

    /** Olvaható formában visszaadja az állapotot. */
    QString getStatusName() const;

signals:
    // Ezeket a signalokat most nem használjuk */
    void statusChanged();
    void timestampChanged();
    void posChanged();
    void xChanged();
    void yChanged();
    void orientChanged();
    void turnChanged();
    void vChanged();
    void aChanged();
    void lightChanged();

private:
    Status _status;
    float _timestamp;
    koord _pos;   /** Pozíció (koordináta) */
    float _v;   /** Sebesség (m/s) */
    float _a;   /** Gyorsulás (m/s2) */
    qint8 _light;

    /** Az állapotok és szöveges verziójuk közti megfeleltetés.
     * A getStatusName() használja. */
    static std::map<int,QString> statusNames;

    /** Beállítja a statusNames értékeit. A konstruktor hívja. */
    void initStatusNames();
};

/** Beburkolja a RobotState.WriteTo metódust. */
QDataStream &operator<<(QDataStream &, const RobotState &);

/** Beburkolja a RobotState.ReadFrom metódust. */
QDataStream &operator>>(QDataStream &, RobotState &);

#endif // ROBOTSTATE_H
