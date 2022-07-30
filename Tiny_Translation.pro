QT += core gui
QT += network
QT += gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += \
    -lGdi32\
    -lUser32

SOURCES += \
    MouseHook.cpp \
    MyTextBrowser.cpp \
    Translator.cpp \
    delimit_window.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    MouseHook.h \
    MyTextBrowser.h \
    Translator.h \
    delimit_window.h \
    mainwindow.h

FORMS += \
    delimit_window.ui \
    mainwindow.ui

# Default rules for deployment.
TARGET=TinyTranslation
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resource/Resource.qrc
