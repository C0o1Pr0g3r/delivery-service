QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addressselector.cpp \
    auxiliaryfunctions.cpp \
    childwindow.cpp \
    clientcreator.cpp \
    clienteditor.cpp \
    clientsearch.cpp \
    database.cpp \
    dbdatamodel.cpp \
    dsdbmapper.cpp \
    employeecreator.cpp \
    employeeeditor.cpp \
    employeesearch.cpp \
    groupboxwithclosebutton.cpp \
    login.cpp \
    mailingcreator.cpp \
    mailingeditor.cpp \
    mailingsearch.cpp \
    main.cpp \
    mainwindow.cpp \
    officesearch.cpp \
    passwordcreator.cpp \
    shparrivedtooffice.cpp \
    shpcreator.cpp \
    shpeditor.cpp \
    shpsearch.cpp \
    stgcreator.cpp \
    tableviewwithmultipleselection.cpp

HEADERS += \
    addressselector.h \
    auxiliaryfunctions.h \
    childwindow.h \
    clientcreator.h \
    clienteditor.h \
    clientsearch.h \
    database.h \
    dbdatamodel.h \
    dsdbmapper.h \
    employeecreator.h \
    employeeeditor.h \
    employeesearch.h \
    groupboxwithclosebutton.h \
    login.h \
    mailingcreator.h \
    mailingeditor.h \
    mailingsearch.h \
    mainwindow.h \
    officesearch.h \
    passwordcreator.h \
    shparrivedtooffice.h \
    shpcreator.h \
    shpeditor.h \
    shpsearch.h \
    stgcreator.h \
    tableviewwithmultipleselection.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

FORMS += \
    addressselector.ui \
    clientcreator.ui \
    clienteditor.ui \
    clientsearch.ui \
    employeecreator.ui \
    employeeeditor.ui \
    employeesearch.ui \
    login.ui \
    mailingcreator.ui \
    mailingeditor.ui \
    mailingsearch.ui \
    mainwindow.ui \
    officesearch.ui \
    passwordcreator.ui \
    shparrivedtooffice.ui \
    shpcreator.ui \
    shpeditor.ui \
    shpsearch.ui \
    stgcreator.ui
