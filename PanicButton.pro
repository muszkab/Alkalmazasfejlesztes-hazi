TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    bsp.c \
    state.c

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    bsp.h

