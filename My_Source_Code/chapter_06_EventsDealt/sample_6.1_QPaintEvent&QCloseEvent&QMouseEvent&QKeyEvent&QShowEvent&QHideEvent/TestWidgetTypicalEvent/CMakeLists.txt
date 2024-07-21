cmake_minimum_required(VERSION 3.5)

project(TestWidgetTypicalEvent VERSION 0.1 LANGUAGES CXX)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_OBJECT_PATH_MAX 2048)
set(CMAKE_OBJECT_NAME_MAX 512)
if (WIN32)
    set(CMAKE_OBJECT_PATH_MAX 2048)
    set(CMAKE_OBJECT_NAME_MAX 512)
endif()
if(CMAKE_HOST_UNIX)
  execute_process( COMMAND getconf PATH_MAX /
    OUTPUT_VARIABLE OBJECT_PATH_MAX OUTPUT_STRIP_TRAILING_WHITESPACE)
  execute_process( COMMAND getconf NAME_MAX /
    OUTPUT_VARIABLE OBJECT_NAME_MAX OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        TestWidgetTypicalEvent.cpp
        TestWidgetTypicalEvent.h
        TestWidgetTypicalEvent.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TestWidgetTypicalEvent
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        resources.qrc
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET TestWidgetTypicalEvent APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(TestWidgetTypicalEvent SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(TestWidgetTypicalEvent
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(TestWidgetTypicalEvent PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.TestWidgetTypicalEvent)
endif()
set_target_properties(TestWidgetTypicalEvent PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS TestWidgetTypicalEvent
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TestWidgetTypicalEvent)
endif()

#源字符集指的是cpp文件中字符串的编码方式
# add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")
#执行字符集指的是exe文件中字符串的编码方式
# add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/execution-charset:utf-8>")
