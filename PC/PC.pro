######################################################################
# Automatically generated by qmake (3.0) So. Sep. 3 17:45:43 2017
######################################################################

CONFIG += qt
CONFIG += c++11
TEMPLATE = app
TARGET = RobotGui
INCLUDEPATH += .
LIBS += Netzwerk/ProzessPiClient.o
LIBS += Netzwerk/Funktion_zum_Umwandeln.o

QT += core gui
QT += widgets

# Input
HEADERS += MessageListener.h \
           ProtocolLibrary.h \
           Robot.h \
			custommainwindow.h \
			DistanceThread.h

SOURCES += main.cpp \
           MessageListener.cpp \
           ProtocolLibrary.cpp \
           Robot.cpp \
			custommainwindow.cpp\
			DistanceThread.cpp
