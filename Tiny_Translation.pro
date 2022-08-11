QT += core gui
QT += network
QT += gui
QT += core5compat
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += \
    -lGdi32\
    -lUser32

SOURCES += \
#    src/*.cpp
   src/MouseHook.cpp \
   src/MyTextBrowser.cpp \
   src/Translator.cpp \
   src/delimit_window.cpp \
   src/main.cpp \
   src/mainwindow.cpp

HEADERS += \
#    include/*.h
    include/MouseHook.h \
    include/MyTextBrowser.h \
    include/Translator.h \
    include/delimit_window.h \
    include/mainwindow.h

FORMS += \
    ui/delimit_window.ui \
    ui/mainwindow.ui

INCLUDEPATH += \
    include

# Default rules for deployment.
TARGET=TinyTranslation
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resource/Resource.qrc
