#include <QDebug>
#include <QQmlContext>
#include <QtQml>
#include "Application.h"

int main(int argc, char *argv[])
{
    qmlRegisterType<RobotState>("com.qmlcomponents", 1, 0, "Robotstate");
    Application app(argc, argv);

    Szimulator app(3333);
    app.start(1);
    return 0;
}
