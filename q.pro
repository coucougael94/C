QT       += core gui network location positioning core-private core5compat

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    datadate.cpp \
    emissaires.cpp \
    locationfetcher.cpp \
    main.cpp \
    mainwindow.cpp \
    notificationhandler.cpp \
    stackmgr.cpp \
    weatherfetcher.cpp

HEADERS += \
    datadate.h \
    emissaires.h \
    locationfetcher.h \
    mainwindow.h \
    notificationhandler.h \
    stackmgr.h \
    weatherfetcher.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
