#pragma once
#ifndef ROBOTSTATEHISTORY_H
#define ROBOTSTATEHISTORY_H
#include <QtCore>
#include <memory>
#include <vector>
#include "RobotState.h"

/**
 * @brief Visszamenőleg tárolja a robot állapotait,
 * valamint innen lehet lekérdezni a legutóbbi érvényes állapotot is.
 */
class RobotStateHistory : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor.
     */
    RobotStateHistory();
    virtual ~RobotStateHistory() = default;

    /**
     * Állapotok listája, melyet a QML oldalhoz lehet kötni.
     * Ez a lista csak pointereket tárol a container-ben lévő elemekre.
     *
     * @warning Ennek QObject* listának kell lennie. A leszármaztatott osztályokra
     * mutató pointereket nem ismeri fel a QML adatkötés.
     */
    QList<QObject*> stateList;

    /** */
    QList<double> XpositionList;

    /** Mutató a legutolsó érvényes állapotra. Az add() frissíti. */
    RobotState *currentState;

    /** Ez a tároló tárolja ténylegesen az állapotokat. (Övé az ownership.) */
    std::vector<std::unique_ptr<RobotState>> container;

    /** A paraméterül kapott állapot egy másolatát a history végéhez fűzi és aktuálissá teszi. */
    void Add(const RobotState& state);

    /** \addtogroup Tárolók a közvetlen megjelenítéshez.
     * Minden elemet tárolnak.
     * Az Add() frissíti.
     *  @{
     */
    QList<int> graphVelocities;
    QList<double> graphPositionX;
    QList<double> graphPositionY;
    QList<int> vectorOrient;
    /** @}*/

signals:
    /** Add() után kiadott signal. */
    void historyChanged();
};

#endif // ROBOTSTATEHISTORY_H
