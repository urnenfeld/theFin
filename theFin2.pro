TARGET = thefin

LIBS += \
    Urho3D/lib/libUrho3D.so \
    -lpthread \
    -ldl \
    -lGL

QMAKE_CXXFLAGS += -std=c++11 -O2

INCLUDEPATH += \
    Urho3D/include \
    Urho3D/include/Urho3D/ThirdParty \

TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += \
    luckey.h \
    mastercontrol.h \
    sceneobject.h \
    spawnmaster.h \
    inputmaster.h \
    player.h \
    controllable.h \
    effectmaster.h \
    localfisher.h \
    fishmaster.h \
    uimaster.h \
    fish.h

SOURCES += \
    luckey.cpp \
    mastercontrol.cpp \
    sceneobject.cpp \
    spawnmaster.cpp \
    inputmaster.cpp \
    player.cpp \
    controllable.cpp \
    effectmaster.cpp \
    localfisher.cpp \
    fishmaster.cpp \
    uimaster.cpp

DISTFILES += \
    LICENSE_TEMPLATE
