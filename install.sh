#!/usr/bin/env bash
#
# install.sh — Install the Designer Dynamic Property Widget wizard
# into the Qt Creator custom wizards directory on Linux or macOS.
#
# Usage:
#   ./install.sh                # install
#   ./install.sh --uninstall    # remove the wizard
#   ./install.sh --dry-run      # show what would happen, do nothing
#   ./install.sh --pause        # always pause before exit
#   ./install.sh --no-pause     # never pause before exit
#   ./install.sh --help         # show this help
#
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
WIZARD_NAME="designer-dynamic-props"
SOURCE_DIR="$SCRIPT_DIR/$WIZARD_NAME"

# ---- Default pause behavior ----
# Pause by default only when stdin is not a terminal. This covers the
# "double-clicked from a file manager" case, where the script must pause
# for the user to see the output. When run from a terminal, the user can
# already see the output and pausing would be annoying.
if [ -t 0 ]; then
    PAUSE_AT_END=0
else
    PAUSE_AT_END=1
fi

# ---- Parse arguments ----
MODE="install"
for arg in "$@"; do
    case "$arg" in
        --uninstall)  MODE="uninstall" ;;
        --dry-run)    MODE="dry-run" ;;
        --pause)      PAUSE_AT_END=1 ;;
        --no-pause)   PAUSE_AT_END=0 ;;
        --help|-h)
            sed -n '2,15p' "$0" | sed 's/^# \{0,1\}//'
            exit 0
            ;;
        *)
            echo "Error: unknown option: $arg" >&2
            echo "Run '$0 --help' for usage." >&2
            exit 1
            ;;
    esac
done

# ---- Pause on exit ----
# 'trap' ensures the pause runs regardless of how the script exits
# (normal completion, error, Ctrl+C, etc.).
cleanup() {
    local exit_code=$?
    if [ "$PAUSE_AT_END" = "1" ]; then
        echo
        # '|| true' prevents the trap from failing if stdin is closed.
        read -r -p "Press Enter to continue..." _ || true
    fi
    exit "$exit_code"
}
trap cleanup EXIT

# ---- Detect OS and pick the target root ----
case "$(uname -s)" in
    Darwin)
        TARGET_ROOT="$HOME/Library/Application Support/QtProject/qtcreator/templates/wizards"
        OS_NAME="macOS"
        ;;
    Linux)
        TARGET_ROOT="$HOME/.config/QtProject/qtcreator/templates/wizards"
        OS_NAME="Linux"
        ;;
    *)
        echo "Error: unsupported operating system: $(uname -s)" >&2
        echo "This script supports Linux and macOS only." >&2
        echo "On Windows, run install.bat instead." >&2
        exit 1
        ;;
esac

TARGET_DIR="$TARGET_ROOT/$WIZARD_NAME"

# ---- Sanity checks ----
if [ "$MODE" != "uninstall" ] && [ ! -d "$SOURCE_DIR" ]; then
    echo "Error: source directory not found: $SOURCE_DIR" >&2
    echo "Make sure the wizard folder '$WIZARD_NAME' exists next to this script." >&2
    exit 1
fi

if [ "$MODE" != "uninstall" ] && [ ! -f "$SOURCE_DIR/wizard.json" ]; then
    echo "Error: '$SOURCE_DIR/wizard.json' not found." >&2
    echo "The wizard folder seems incomplete." >&2
    exit 1
fi

# ---- Print plan ----
echo "Designer Dynamic Property Widget Wizard"
echo "  OS          : $OS_NAME"
echo "  Source      : $SOURCE_DIR"
echo "  Target      : $TARGET_DIR"
echo

# ---- Dry-run: stop here ----
if [ "$MODE" = "dry-run" ]; then
    if [ -d "$TARGET_DIR" ]; then
        echo "[dry-run] Would remove existing: $TARGET_DIR"
    fi
    echo "[dry-run] Would copy: $SOURCE_DIR/ -> $TARGET_DIR/"
    echo "[dry-run] Done. No changes were made."
    exit 0
fi

# ---- Uninstall ----
if [ "$MODE" = "uninstall" ]; then
    if [ ! -d "$TARGET_DIR" ]; then
        echo "Nothing to uninstall: $TARGET_DIR does not exist."
        exit 0
    fi
    rm -rf "$TARGET_DIR"
    echo "Removed: $TARGET_DIR"
    echo "Restart Qt Creator to complete the uninstall."
    exit 0
fi

# ---- Install ----
mkdir -p "$TARGET_ROOT"

if [ -d "$TARGET_DIR" ]; then
    echo "Existing installation found. Replacing it."
    rm -rf "$TARGET_DIR"
fi

cp -R "$SOURCE_DIR" "$TARGET_DIR"

echo "Installed successfully."
echo
echo "Next steps:"
echo "  1. Restart Qt Creator."
echo "  2. Open: File > New Project"
echo "  3. Look for: Qt 4 Designer Custom Widget"
echo "     in the \"Other Project\" group:"
echo "     -> Qt 4 Designer Custom Widget with Dynamic Props"
echo
echo "To uninstall later, run: $0 --uninstall"