# common.pri
#
# Shared build configuration for both subprojects.

COMMON_ROOT = $$shadowed($$PWD)

COMMON_BIN_DIR = $$COMMON_ROOT/bin
COMMON_OBJ_DIR = $$COMMON_ROOT/obj
COMMON_MOC_DIR = $$COMMON_ROOT/moc

# The 'd' suffix for debug builds. We control this ourselves so both
# subprojects agree - qmake's automatic suffix would be disabled below.
CONFIG(debug, debug|release) {
    COMMON_TARGET_SUFFIX = d
} else {
    COMMON_TARGET_SUFFIX =
}
