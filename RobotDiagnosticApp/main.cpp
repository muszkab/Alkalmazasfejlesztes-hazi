#include <QDebug>
#include <QQmlContext>
#include <QtQml>
#include "Application.h"

int main(int argc, char *argv[])
{
    Application app(argc, argv);
    return app.exec();
}
