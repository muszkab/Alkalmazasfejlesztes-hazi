#pragma once
#ifndef STVAPPLICATION_H
#define STVAPPLICATION_H
#include <QApplication>
#include <QQmlApplicationEngine>
#include "szimulator.h"
#include "MainWindowsEventHandling.h"
#include "RobotProxy.h"
#include "Communication/CommunicationTcpSocketClient.h"
#include "RobotStateHistory.h"

/**
 * @brief Alkalmazás osztály. A main() példányosítja és indítja el.
 *
 * A konstruktor elvégzi az alapvető példányosításokat és a signalok összekapcsolását.
 */
class Application : public QApplication
{
public:
    /** Konstruktor. Alapvető példányosítások és signal bekötések. */
    Application(int & argc, char ** argv);
    ~Application() = default;

private:
    Szimulator simulator;
    QQmlApplicationEngine engine;
    RobotStateHistory history;
    CommunicationTcpSocketClient communication;
    RobotProxy robot;
    MainWindowsEventHandling handler;
};

#endif // STVAPPLICATION_H
