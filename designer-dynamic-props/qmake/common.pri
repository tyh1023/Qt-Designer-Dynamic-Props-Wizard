# common.pri
#
# Shared build configuration for both subprojects.

COMMON_ROOT = $$shadowed($$PWD)

COMMON_BIN_DIR = $$COMMON_ROOT/bin
COMMON_OBJ_DIR = $$COMMON_ROOT/obj
COMMON_MOC_DIR = $$COMMON_ROOT/moc

# Determine whether it is a Profile build (Release + debug symbols)
profile_build = false
contains(CONFIG, force_debug_info):profile_build = true
contains(CONFIG, separate_debug_info):profile_build = true

# Determine whether it is a Debug build
debug_build = false
CONFIG(debug, debug|release):debug_build = true

# Set the suffix according to the construction type
debug_build {
    COMMON_TARGET_SUFFIX = d
} else:profile_build {
    COMMON_TARGET_SUFFIX =
    # Profiles do not have suffixes, but specific configurations
} else {
    COMMON_TARGET_SUFFIX =
}
