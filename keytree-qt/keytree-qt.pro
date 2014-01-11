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

SOURCES += main.cpp\
        mainwindow.cpp \
    keytree/logger.cpp \
    keytree/keytree.cpp \
    keytree/CoinClasses/hdkeys.cpp \
    elasticnodes/node.cpp \
    elasticnodes/graphwidget.cpp \
    elasticnodes/edge.cpp

HEADERS  += mainwindow.h \
    keytree/stringutils.h \
    keytree/logger.h \
    keytree/keytree.h \
    keytree/CoinClasses/uchar_vector.h \
    keytree/CoinClasses/typedefs.h \
    keytree/CoinClasses/secp256k1.h \
    keytree/CoinClasses/hdkeys.h \
    keytree/CoinClasses/hash.h \
    keytree/CoinClasses/encodings.h \
    keytree/CoinClasses/BigInt.h \
    keytree/CoinClasses/Base58Check.h \
    elasticnodes/node.h \
    elasticnodes/graphwidget.h \
    elasticnodes/edge.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    keytree/HDWallet.1
