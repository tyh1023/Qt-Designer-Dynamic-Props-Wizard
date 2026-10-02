# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Planned

- Qt Quick (QML) widget variant
- Visual rule editor for Qt Designer
- Additional build systems (Meson, Qbs)

## [1.0.0] - 2026-10-03

### Added

- **Qt Creator custom wizard** for generating Qt Designer custom widgets
  with runtime-driven property rules.
- **Runtime property rules** — control visibility, editability, reset
  availability, attributes, and values of properties in the Qt Designer
  property editor, from C++ predicates registered at construction time.
- **Widget-agnostic mechanism** — the rule engine (`DesignerPropertyPolicy`)
  works with any widget implementing the `QDesignerDynamicProperties`
  interface, not just the generated one.
- **Static widget library** — the widget library has no Qt Designer
  dependency and can be linked by any application.
- **Build system support**:
  - qmake (Qt 5 and Qt 6)
  - CMake with Qt 5 compatibility
  - CMake with Qt 6
- **Bilingual wizard UI** — English default, Simplified Chinese localized.
- **Installer scripts** — `install.sh` (Linux, macOS) and `install.bat`
  (Windows), with `--uninstall` / `/uninstall` and `--dry-run` / `/dryrun`
  options.
- **Shared build configuration** — `common.pri` ensures both the widget
  library and the plugin library agree on output paths and target suffixes.

### Fixed

- Placeholder syntax unified to Qt Creator's `%{...}` form (previous drafts
  mixed `%Var%` and `@Var@` syntax).
- Backslash escaping in template files corrected to match Qt Creator's
  template processing (`\\` in templates becomes `\` in generated files).
- Debug/Release library naming is now explicit through
  `COMMON_TARGET_SUFFIX` and `CONFIG += no_debug_suffix`.

[Unreleased]: https://github.com/tyh1023/Qt-Designer-Dynamic-Props-Wizard/compare/v1.0.0...HEAD
[1.0.0]: https://github.com/tyh1023/Qt-Designer-Dynamic-Props-Wizard/releases/tag/v1.0.0