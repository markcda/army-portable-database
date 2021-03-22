QT += core gui xml widgets
CONFIG += c++2a
SOURCES += \
    data.cpp \
    databrickwidget.cpp \
    addnodedialog.cpp \
    docs/xmldatabase.cpp \
    documentwidget.cpp \
    main.cpp \
    documentswindow.cpp \
    nodescollection.cpp
HEADERS += \
    data.h \
    databrickwidget.h \
    addnodedialog.h \
    docs/databrick.h \
    docs/document.h \
    docs/xmldatabase.h \
    documentswindow.h \
    documentwidget.h \
    nodescollection.h
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
RESOURCES += \
  resources.qrc
