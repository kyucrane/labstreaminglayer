######################################################################
# Automatically generated by qmake (2.01a) Tue Apr 7 15:57:45 2015
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += . zmq/include
INCLUDEPATH += . zmq/include

# Input
HEADERS += client.h \
           mainwindow.h \
           tcp_client.h \
           zmq_client.h \
           zmq/include/zmq.h \
           zmq/include/zmq.hpp \
           zmq/include/zmq_utils.h
FORMS += mainwindow.ui
SOURCES += main.cpp mainwindow.cpp
INCLUDEPATH += ../../LSL/liblsl/include /usr/local/include
LIBS += -L./OSX -L/opt/local/lib -L/usr/local/lib -L./ -L./zmq/bin/mac -llsl64 -lboost_thread-mt -lboost_system-mt -lboost_chrono-mt -lzmq
