#include <QGuiApplication>
#include <QQmlApplicationEngine>
//#include <QDebug>
#include "szimulator.h"

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    Szimulator app(3333);
    app.start(1);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return 0;
}

