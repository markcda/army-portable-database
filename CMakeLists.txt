cmake_minimum_required(VERSION 3.5)
project(cclc-portable-database)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(Qt5 COMPONENTS Core Gui Widgets Xml REQUIRED)

include_directories(
    .
    docs
    ex
)

set (SRCS
    docs/database.h
    docs/databrick.h
    docs/document.h
    docs/xmldatabase.cpp
    docs/xmldatabase.h
    ex/exporter.cpp
    ex/exporter.h
    adddocumentdialog.cpp
    adddocumentdialog.h
    addnodedialog.cpp
    addnodedialog.h
    data.cpp
    data.h
    databrickwidget.cpp
    databrickwidget.h
    documentswindow.cpp
    documentswindow.h
    documentwidget.cpp
    documentwidget.h
    editdocumentdialog.cpp
    editdocumentdialog.h
    editnodedialog.cpp
    editnodedialog.h
    firsttimesetupdialog.cpp
    firsttimesetupdialog.h
    imageviewer.cpp
    imageviewer.h
    main.cpp
    movedialog.cpp
    movedialog.h
    navbar.cpp
    navbar.h
    nodescollection.cpp
    nodescollection.h
    searchdialog.cpp
    searchdialog.h
)


add_executable(${CMAKE_PROJECT_NAME} ${SRCS} resources.qrc)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Xml)
