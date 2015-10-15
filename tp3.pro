INCLUDEPATH += $$PWD
SOURCES += $$PWD/openglwindow.cpp \
    serveur.cpp \
    client.cpp \
    serveurwindow.cpp
HEADERS += $$PWD/openglwindow.h \
    serveur.h \
    client.h \
    serveurwindow.h

SOURCES += \
    main.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/gui/openglwindow
INSTALLS += target
QMAKE_MAC_SDK = macosx10.11

RESOURCES += \
    gestionnaire.qrc
QT += network
TARGET = QTcpSocket

QT += widgets

CONFIG += c++11
