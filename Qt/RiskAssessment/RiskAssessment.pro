include(RiskAssessment.pri)

CONFIG += lrelease
CONFIG += embed_translations
CONFIG+= static

SOURCES += $$PWD/main.cpp \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
