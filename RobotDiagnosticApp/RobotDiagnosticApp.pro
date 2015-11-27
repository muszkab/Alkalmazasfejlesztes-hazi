TEMPLATE = app

QT += qml quick
CONFIG += c++11

QMAKE_CXXFLAGS_CXX11    = -std=c++1y

QT += qml quick widgets
QT += serialport

QMAKE_CC = gcc-4.9
QMAKE_CXX = g++-4.9

SOURCES += main.cpp \
    szimulator.cpp \
    RobotState.cpp \
    Communication/Communication.cpp \
    Communication/CommunicationSerialPort.cpp \
    Communication/CommunicationTcpSocket.cpp \
    Communication/CommunicationTcpSocketClient.cpp \
    Communication/CommunicationTcpSocketServer.cpp \
    MainWindowsEventHandling.cpp \
    RobotProxy.cpp \
    RobotStateHistory.cpp \
    Application.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    szimulator.h \
    RobotState.h \
    Communication/Communication.h \
    Communication/CommunicationSerialPort.h \
    Communication/CommunicationTcpSocket.h \
    Communication/CommunicationTcpSocketClient.h \
    Communication/CommunicationTcpSocketServer.h \
    MainWindowsEventHandling.h \
    RobotProxy.h \
    RobotStateHistory.h \
    Application.h

DISTFILES +=

