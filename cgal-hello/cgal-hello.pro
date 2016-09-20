TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

LIBS += \
      -lCGAL \
      -lCGAL_Core \
      -lCGAL_ImageIO \
      -lCGAL_Qt5
