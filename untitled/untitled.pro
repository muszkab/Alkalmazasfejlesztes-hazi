TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    Communication/Communication.cpp \
    Communication/CommunicationSerialPort.cpp \
    Communication/CommunicationTcpSocket.cpp \
    Communication/CommunicationTcpSocketClient.cpp \
    Communication/CommunicationTcpSocketServer.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    Communication/Communication.h \
    Communication/CommunicationSerialPort.h \
    Communication/CommunicationTcpSocket.h \
    Communication/CommunicationTcpSocketClient.h \
    Communication/CommunicationTcpSocketServer.h

