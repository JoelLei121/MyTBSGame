QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Sources/main.cpp \
    Sources/Algorithm.cpp \
    Sources/AnimationManager.cpp \
    Sources/BattleField.cpp \
    Sources/BattleSystem.cpp \
    Sources/GroundUnit.cpp \
    Sources/MenuUi.cpp \
    Sources/RightUi.cpp \
    Sources/RoleUnit.cpp \
    Sources/RoleUnitStatus.cpp \
    Sources/Level.cpp \
    Sources/LevelUi.cpp \
    Sources/LevelCreator.cpp \
    Sources/tutorial.cpp

HEADERS += \
    Headers/Algorithm.h \
    Headers/AnimationManager.h \
    Headers/BattleField.h \
    Headers/BattleSystem.h \
    Headers/DataPackage.h \
    Headers/GroundUnit.h \
    Headers/MenuUi.h \
    Headers/RightUi.h \
    Headers/RoleUnit.h \
    Headers/RoleUnitStatus.h \
    Headers/Level.h \
    Headers/LevelUi.h \
    Headers/LevelCreator.h \
    Headers/tutorial.h


FORMS += \
    tutorial.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Assets.qrc
