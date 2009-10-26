# -------------------------------------------------
# Project created by QtCreator 2009-10-13T23:01:36
# -------------------------------------------------
TARGET = comm_nt
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    userchat.cpp \
    ReceiverThread.cpp \
    Utilities.cpp \
    stdafx.cpp \
    Socket.cpp \
    CommonClasses.cpp \
    Client.cpp \
    loginform.cpp
HEADERS += mainwindow.h \
    userchat.h \
    ReceiverThread.h \
    Utilities.h \
    targetver.h \
    stdafx.h \
    Socket.h \
    CommonClasses.h \
    Client.h \
    BasicSystemClasses.h \
    loginform.h
FORMS += mainwindow.ui \
    userchat.ui \
    loginform.ui
LIBS += -L/ \
    -lWS2_32
