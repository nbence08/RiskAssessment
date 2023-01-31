QT += core gui designer widgets

CONFIG += c++20

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    $$PWD/commandByView/additemcommand.cpp \
    $$PWD/commandByView/addzonecommand.cpp \
    $$PWD/commandByView/connectwithzonecommand.cpp \
    $$PWD/commandByView/createassociationcommand.cpp \
    $$PWD/commandByView/grantconnectioncommand.cpp \
    $$PWD/commandByView/grantlprivcommand.cpp \
    $$PWD/commandByView/grantpaccesscommand.cpp \
    $$PWD/entitiesdialog.cpp \
    $$PWD/io/riskserializator.cpp \
    $$PWD/io/serializationdispatchtable.cpp \
    $$PWD/io/serializationdispatchtablebuilder.cpp \
    $$PWD/mainwindow.cpp \
    $$PWD/model/actor.cpp \
    $$PWD/model/connection.cpp \
    $$PWD/model/device.cpp \
    $$PWD/model/employee.cpp \
    $$PWD/model/logicalzone.cpp \
    $$PWD/model/lpriv.cpp \
    $$PWD/model/model.cpp \
    $$PWD/model/operation.cpp \
    $$PWD/model/paccess.cpp \
    $$PWD/model/path.cpp \
    $$PWD/model/physicalzone.cpp \
    $$PWD/model/role.cpp \
    $$PWD/model/service.cpp \
    $$PWD/model/serviceclass.cpp \
    $$PWD/model/visitor.cpp \
    $$PWD/riskeditcontroller.cpp \
    $$PWD/rolesdialog.cpp \
    $$PWD/view/actorgraphicsview.cpp \
    $$PWD/view/associationconnector.cpp \
    $$PWD/view/connectionconnector.cpp \
    $$PWD/view/connector.cpp \
    $$PWD/view/devicegraphicsview.cpp \
    $$PWD/view/diagramitem.cpp \
    $$PWD/view/diagramitemlabel.cpp \
    $$PWD/view/employeegraphicsview.cpp \
    $$PWD/view/logicalzonegraphicsview.cpp \
    $$PWD/view/lprivconnector.cpp \
    $$PWD/view/operationgraphicsview.cpp \
    $$PWD/view/paccessconnector.cpp \
    $$PWD/view/physicalzonegraphicsview.cpp \
    $$PWD/view/riskeditscene.cpp \
    $$PWD/view/rolegraphicsview.cpp \
    $$PWD/view/serviceclassgraphicsview.cpp \
    $$PWD/view/servicegraphicsview.cpp \
    $$PWD/view/viewvisitor.cpp \
    $$PWD/view/zoneconnector.cpp \
    $$PWD/zonesedit.cpp

HEADERS += \
    $$PWD/commandByView/additemcommand.hpp \
    $$PWD/commandByView/addzonecommand.hpp \
    $$PWD/commandByView/connectwithzonecommand.hpp \
    $$PWD/commandByView/createassociationcommand.hpp \
    $$PWD/commandByView/grantconnectioncommand.hpp \
    $$PWD/commandByView/grantlprivcommand.hpp \
    $$PWD/commandByView/grantpaccesscommand.hpp \
    $$PWD/core/command.hpp \
    $$PWD/entitiesdialog.hpp \
    $$PWD/io/riskserializator.hpp \
    $$PWD/io/serializationdispatchtable.hpp \
    $$PWD/io/serializationdispatchtablebuilder.hpp \
    $$PWD/mainwindow.hpp \
    $$PWD/model/actor.hpp \
    $$PWD/model/allmodels.hpp \
    $$PWD/model/connection.hpp \
    $$PWD/model/device.hpp \
    $$PWD/model/employee.hpp \
    $$PWD/model/logicalzone.hpp \
    $$PWD/model/lpriv.hpp \
    $$PWD/model/model.hpp \
    $$PWD/model/operation.hpp \
    $$PWD/model/paccess.hpp \
    $$PWD/model/path.hpp \
    $$PWD/model/physicalzone.hpp \
    $$PWD/model/role.hpp \
    $$PWD/model/serializable.hpp \
    $$PWD/model/service.hpp \
    $$PWD/model/serviceclass.hpp \
    $$PWD/model/visitor.hpp \
    $$PWD/riskeditcontroller.hpp \
    $$PWD/rolesdialog.hpp \
    $$PWD/view/actorgraphicsview.hpp \
    $$PWD/view/additemcommand.hpp \
    $$PWD/view/allconnectors.hpp \
    $$PWD/view/alldiagramitems.hpp \
    $$PWD/view/allviews.hpp \
    $$PWD/view/associationconnector.hpp \
    $$PWD/view/connectionconnector.hpp \
    $$PWD/view/connector.hpp \
    $$PWD/view/connectwithzonecommand.hpp \
    $$PWD/view/deletablediagramelement.hpp \
    $$PWD/view/devicegraphicsview.hpp \
    $$PWD/view/devicegraphicsview.hpp \
    $$PWD/view/diagramitem.hpp \
    $$PWD/view/diagramitemlabel.hpp \
    $$PWD/view/employeegraphicsview.hpp \
    $$PWD/view/grantconnectioncommand.hpp \
    $$PWD/view/grantpaccesscommand.hpp \
    $$PWD/view/logicalzonegraphicsview.hpp \
    $$PWD/view/lprivconnector.hpp \
    $$PWD/view/operationgraphicsview.hpp \
    $$PWD/view/paccessconnector.hpp \
    $$PWD/view/physicalzonegraphicsview.hpp \
    $$PWD/view/riskeditscene.hpp \
    $$PWD/view/rolegraphicsview.hpp \
    $$PWD/view/serviceclassgraphicsview.hpp \
    $$PWD/view/servicegraphicsview.hpp \
    $$PWD/view/viewvisitor.hpp \
    $$PWD/view/zoneconnector.hpp \
    $$PWD/zonesedit.hpp

FORMS += \
    $$PWD/entitiesdialog.ui \
    $$PWD/mainwindow.ui \
    $$PWD/rolesdialog.ui \
    $$PWD/zonesedit.ui

INCLUDEPATH += $$PWD/view
INCLUDEPATH += $$PWD/model
INCLUDEPATH += $$PWD/io
INCLUDEPATH += $$PWD/commandByView
INCLUDEPATH += $$PWD/core
INCLUDEPATH += $$PWD

TRANSLATIONS += \
    $$PWD/RiskAssesment_en_US.ts

RESOURCES += \
    $$PWD/Icons.qrc
