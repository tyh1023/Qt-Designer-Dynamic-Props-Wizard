include(../common.pri)

TEMPLATE = lib
CONFIG  += staticlib c++17

# We control the debug suffix ourselves.
CONFIG -= debug_and_release_target
CONFIG += no_debug_suffix

TARGET = %{ProjectName}$${COMMON_TARGET_SUFFIX}

profile_build {
    QMAKE_CXXFLAGS += -g
    # Ensure that debugging information is generated
    QMAKE_LFLAGS += -g
    # Other Profile Specific Settings
}

QT += widgets

CONFIG += utf8_source
msvc: QMAKE_CXXFLAGS += /utf-8
gcc|clang: QMAKE_CXXFLAGS += -finput-charset=UTF-8 -fexec-charset=UTF-8

HEADERS += \\
    %{WidgetClassName}.h \\
    lib/designerdynamicproperties.h \\
    lib/designerpropertypolicy.h

SOURCES += \\
    %{WidgetClassName}.cpp \\
    lib/designerpropertypolicy.cpp

INCLUDEPATH += . \\
    lib

DESTDIR     = $$COMMON_BIN_DIR
OBJECTS_DIR = $$COMMON_OBJ_DIR/%{ProjectName}
MOC_DIR     = $$COMMON_MOC_DIR/%{ProjectName}
