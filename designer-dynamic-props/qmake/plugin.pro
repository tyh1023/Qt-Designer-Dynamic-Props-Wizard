include(../common.pri)

TEMPLATE = lib
CONFIG  += plugin c++17

CONFIG -= debug_and_release_target
CONFIG += no_debug_suffix

TARGET = %{ProjectName}Plugin

profile_build {
    QMAKE_CXXFLAGS += -g
    # Ensure that debugging information is generated
    QMAKE_LFLAGS += -g
    # Other Profile Specific Settings
}

QT += designer widgets uiplugin

CONFIG += utf8_source
msvc: QMAKE_CXXFLAGS += /utf-8
gcc|clang: QMAKE_CXXFLAGS += -finput-charset=UTF-8 -fexec-charset=UTF-8

HEADERS += \\
    %{WidgetClassName}Plugin.h \\
    dynamicpropertysheetwrapper.h \\
    dynamicpropertysheetwrapperfactory.h

SOURCES += \\
    %{WidgetClassName}Plugin.cpp \\
    dynamicpropertysheetwrapper.cpp \\
    dynamicpropertysheetwrapperfactory.cpp

INCLUDEPATH += . \\
    ../%{ProjectName} \\
    ../%{ProjectName}/lib

# Use the same suffix variable - guaranteed to match widget library's name.
LIBS += -L$$COMMON_BIN_DIR -l%{ProjectName}$${COMMON_TARGET_SUFFIX}

DESTDIR     = $$COMMON_BIN_DIR
OBJECTS_DIR = $$COMMON_OBJ_DIR/%{ProjectName}Plugin
MOC_DIR     = $$COMMON_MOC_DIR/%{ProjectName}Plugin

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS   += target
