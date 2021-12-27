cmake_minimum_required(VERSION 3.14...3.18)

project(
  minVC
  LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

add_subdirectory(QGrypho)

set(PROJECT_SOURCES
    main.cpp
    minvc_core.cpp
    minvc_core.h
    exp.cpp
    log_comb.cpp
    clique.cpp
    vc_ui.cpp
    vc_ui.h
    vc_ui.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(minVC
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(minVC SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(minVC
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(minVC PRIVATE Qt${QT_VERSION_MAJOR}::Widgets QGrypho)
