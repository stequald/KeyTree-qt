#-------------------------------------------------
#
# Project created by QtCreator 2013-12-27T11:03:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = keytree-qt
TEMPLATE = app

QMAKE_CXXFLAGS = -std=c++0x
LIBS += -lcrypto
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    elasticnodes/node.cpp \
    elasticnodes/graphwidget.cpp \
    elasticnodes/edge.cpp \
    keynode/keynode.cpp \
    keynode/logger.cpp \
    keynode/CoinClasses/hdkeys.cpp \
    keytreeeditdialog.cpp \
    keytreeutil.cpp \
    guiutil.cpp
    keytreeutil.cpp

HEADERS  += mainwindow.h \
    elasticnodes/node.h \
    elasticnodes/graphwidget.h \
    elasticnodes/edge.h \
    keynode/keynode.h \
    keynode/logger.h \
    keynode/stringutils.h \
    keynode/CoinClasses/Base58Check.h \
    keynode/CoinClasses/BigInt.h \
    keynode/CoinClasses/encodings.h \
    keynode/CoinClasses/hash.h \
    keynode/CoinClasses/hdkeys.h \
    keynode/CoinClasses/secp256k1.h \
    keynode/CoinClasses/typedefs.h \
    keynode/CoinClasses/uchar_vector.h \
    keytreeeditdialog.h \
    keytreeutil.h \
    guiutil.h

FORMS    += mainwindow.ui \
    keytreeeditdialog.ui
