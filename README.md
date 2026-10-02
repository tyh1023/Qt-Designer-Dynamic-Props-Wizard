# Designer Dynamic Property Widget Wizard

> A Qt Creator custom wizard that generates Qt Designer custom widget projects
> with **runtime-driven property rules**.

[English](#english) · [简体中文](#简体中文)

---

## English

### Table of Contents

- [What it does](#what-it-does)
- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Adding your own rules](#adding-your-own-rules)
- [Repository layout](#repository-layout)
- [Uninstallation](#uninstallation)
- [License](#license)

### What it does

Qt Designer's property editor is normally static: whether a property is
visible, editable, or resettable is decided at compile time by `DESIGNABLE`,
`EDITABLE`, and similar macros.

This wizard generates a project where the property editor becomes **dynamic**:
visibility, editability, reset availability, and even displayed values can be
driven by **runtime rules** written in C++. Change one property, and other
properties instantly appear, hide, grey out, or update.

This is achieved by a small interface, `QDesignerDynamicProperties`, plus a
rule engine (`DesignerPropertyPolicy`) and a Qt Designer extension wrapper.

### Features

- **Runtime property rules** — control visibility, editability, reset
  availability, and values from C++ predicates
- **Widget-agnostic mechanism** — the rule engine works with any widget that
  implements `QDesignerDynamicProperties`, not just the generated one
- **Static widget library** — the widget library has **no** Qt Designer
  dependency and can be linked by any application
- **qmake and CMake** — supports Qt 5 and Qt 6 (with dedicated Qt 6 CMake)
- **Bilingual UI** — English default, Simplified Chinese localized
- **Zero configuration** — build the generated project and the widget
  appears in Qt Designer

### Requirements

- Qt 5.15 or newer, or Qt 6.5 or newer
- Qt Creator 6 or newer (for custom wizard support)
- A C++17 compiler
- qmake (Qt 5 or Qt 6) and/or CMake 3.16+

### Installation

#### Quick install (recommended)

1. Clone or download this repository
2. Run the install script:
   - **Windows**: double-click `install.bat`
   - **Linux / macOS**: `chmod +x install.sh && ./install.sh`
3. Restart Qt Creator

#### Manual install

Copy the `designer-dynamic-props/` folder into your Qt Creator custom
wizards directory:

| Platform | Path                                                         |
| -------- | ------------------------------------------------------------ |
| Windows  | `%APPDATA%\QtProject\qtcreator\templates\wizards\`           |
| Linux    | `~/.config/QtProject/qtcreator/templates/wizards/`           |
| macOS    | `~/Library/Application Support/QtProject/qtcreator/templates/wizards/` |

The final path should look like:

```
<wizards-dir>/designer-dynamic-props/wizard.json
```

Restart Qt Creator after copying.

### Usage

1. In Qt Creator: **File → New Project**
2. Under **Qt 4 Designer Custom Widget**, select
   **Qt 4 Designer Custom Widget with Dynamic Props**
3. Fill in:
   - Project name (e.g. `MyWidget`)
   - Build system (qmake / CMake / CMake Qt6)
   - Widget class name (e.g. `MyWidget`)
   - Base class (e.g. `QWidget`)
   - Designer group and display name
4. Qt Creator generates a project with this structure:

   ```
   MyWidget/
   ├── MyWidgetAll.pro            (or CMakeLists.txt)
   ├── MyWidget/                  ← widget library (no Designer dependency)
   │   ├── MyWidget.h
   │   ├── MyWidget.cpp
   │   └── lib/
   └── MyWidgetPlugin/            ← Designer plugin library
       ├── MyWidgetPlugin.h
       ├── MyWidgetPlugin.cpp
       └── dynamicpropertysheetwrapper.h/.cpp
   ```

5. Build the top-level project
6. Run `make install` (qmake) or `cmake --install` (CMake) to place the
   plugin in Qt Designer's plugin directory
7. Open Qt Designer — the widget appears in the group you chose

### Adding your own rules

The generated `MyWidget` is intentionally empty: no demonstration
properties, no sample rules. To add a rule:

**Step 1** — Add a property to your widget's header:

```cpp
Q_PROPERTY(bool enableAdvanced READ enableAdvanced
           WRITE setEnableAdvanced NOTIFY enableAdvancedChanged
           DESIGNABLE true)
Q_PROPERTY(int advancedValue READ advancedValue
           WRITE setAdvancedValue DESIGNABLE true)
```

**Step 2** — Register a rule in the constructor:

```cpp
m_policy->setVisibilityRule(
    "advancedValue",                          // property to control
    [this] { return m_enableAdvanced; },      // predicate
    {"enableAdvanced"}                        // dependency list
);
```

**Step 3** — Notify the policy when a dependency changes:

```cpp
connect(this, &MyWidget::enableAdvancedChanged,
        m_policy, &DesignerPropertyPolicy::reevaluate);
```

**Rule types available**:

| Method              | Effect in the property editor              |
| ------------------- | ------------------------------------------ |
| `setVisibilityRule` | Show / hide the property                   |
| `setEnabledRule`    | Enable / grey out the property             |
| `setResetRule`      | Show / hide the property's reset button    |
| `setAttributeRule`  | Treat the property as a Designer attribute |
| `setValueOverride`  | Substitute the value shown in the editor   |

**Important**: the dependency list is required. Without it, changing a
property will not re-evaluate the rule, and the property editor will not
update.

### Repository layout

```
designer-dynamic-props-wizard/
├── README.md                    ← this file
├── LICENSE                      ← MIT
├── install.sh                   ← Linux / macOS installer
├── install.bat                  ← Windows installer
└── designer-dynamic-props/      ← the wizard (this is what gets installed)
    ├── wizard.json
    ├── widget/
    │   ├── widget.h
    │   └── widget.cpp
    ├── plugin/
    │   ├── plugin.h
    │   └── plugin.cpp
    ├── lib/
    │   ├── designerdynamicproperties.h        ← contract (QtCore only)
    │   ├── designerpropertypolicy.h/.cpp      ← rule engine (QtCore only)
    │   ├── dynamicpropertysheetwrapper.h/.cpp ← Designer wrapper
    │   └── dynamicpropertysheetwrapperfactory.h/.cpp
    ├── qmake/
    │   ├── all.pro
    │   ├── widget.pro
    │   └── plugin.pro
    ├── cmake5/
    │   └── *.CMakeLists.txt       ← Qt 5 compatibility
    └── cmake6/
        └── *.CMakeLists.txt       ← Qt 6 only
```

### Uninstallation

- **Windows**: `install.bat /uninstall`
- **Linux / macOS**: `./install.sh --uninstall`

Or manually delete the `designer-dynamic-props/` folder from your Qt
Creator wizards directory. Restart Qt Creator.

### License

MIT — see [LICENSE](LICENSE). You are free to use, modify, and redistribute
the wizard and any projects it generates without restriction.

---

## 简体中文

### 目录

- [这是什么](#这是什么)
- [特性](#特性)
- [系统要求](#系统要求)
- [安装](#安装)
- [使用](#使用)
- [添加你自己的规则](#添加你自己的规则)
- [仓库结构](#仓库结构)
- [卸载](#卸载)
- [许可证](#许可证)

### 这是什么

Qt Designer 的属性编辑器通常是静态的：某个属性是否可见、是否可编辑、
是否有重置按钮，由编译期的 `DESIGNABLE`、`EDITABLE` 等宏决定。

本向导生成的项目让属性编辑器变得**动态**：可见性、可编辑性、重置按钮的
可用性、乃至显示的值，都可以由 C++ 编写的**运行期规则**驱动。改变一个
属性，其他属性立即出现、隐藏、变灰或更新。

这通过一个小接口 `QDesignerDynamicProperties`、一个规则引擎
（`DesignerPropertyPolicy`）和一个 Qt Designer 扩展包装器实现。

### 特性

- **运行期属性规则** — 由 C++ 谓词控制属性的显示/隐藏、可编辑/变灰、
  重置按钮可用性、以及显示值
- **通用机制** — 规则引擎对任何实现 `QDesignerDynamicProperties` 接口的
  控件有效，不局限于向导生成的那个
- **静态控件库** — 控件库**不依赖** Qt Designer，可以被任何应用链接使用
- **qmake 与 CMake** — 支持 Qt 5 和 Qt 6（提供独立的 Qt 6 CMake 版本）
- **双语界面** — 英文默认，简体中文本地化
- **零配置** — 编译生成的项目，控件即出现在 Qt Designer 中

### 系统要求

- Qt 5.15 或更新版本，或 Qt 6.5 或更新版本
- Qt Creator 6 或更新版本（自定义向导支持）
- 支持 C++17 的编译器
- qmake（Qt 5 或 Qt 6）和/或 CMake 3.16+

### 安装

#### 快速安装（推荐）

1. 克隆或下载本仓库
2. 运行安装脚本：
   - **Windows**：双击 `install.bat`
   - **Linux / macOS**：`chmod +x install.sh && ./install.sh`
3. 重启 Qt Creator

#### 手动安装

将 `designer-dynamic-props/` 文件夹复制到 Qt Creator 的向导目录：

| 平台    | 路径                                                         |
| ------- | ------------------------------------------------------------ |
| Windows | `%APPDATA%\QtProject\qtcreator\templates\wizards\`           |
| Linux   | `~/.config/QtProject/qtcreator/templates/wizards/`           |
| macOS   | `~/Library/Application Support/QtProject/qtcreator/templates/wizards/` |

最终路径应类似：

```
<向导目录>/designer-dynamic-props/wizard.json
```

复制后重启 Qt Creator。

### 使用

1. 在 Qt Creator 中：**文件 → 新建项目**
2. 在 **Qt 4 设计师自定义控件** 分类下选择
   **Qt 4 设计师自定义动态属性控件**
3. 填写：
   - 项目名称（例如 `MyWidget`）
   - 构建系统（qmake / CMake / CMake Qt6）
   - 控件类名（例如 `MyWidget`）
   - 基类（例如 `QWidget`）
   - Designer 分组和显示名
4. Qt Creator 会生成如下结构的项目：

   ```
   MyWidget/
   ├── MyWidgetAll.pro            （或 CMakeLists.txt）
   ├── MyWidget/                  ← 控件库（不依赖 Designer）
   │   ├── MyWidget.h
   │   ├── MyWidget.cpp
   │   └── lib/
   └── MyWidgetPlugin/            ← Designer 插件库
       ├── MyWidgetPlugin.h
       ├── MyWidgetPlugin.cpp
       └── dynamicpropertysheetwrapper.h/.cpp
   ```

5. 编译顶层项目
6. 运行 `make install`（qmake）或 `cmake --install`（CMake），将插件
   放入 Qt Designer 的插件目录
7. 打开 Qt Designer — 控件出现在你选择的分组下

### 添加你自己的规则

生成的 `MyWidget` 故意留空：没有演示属性，没有示例规则。添加规则的
步骤如下：

**第 1 步** — 在控件头文件中添加属性：

```cpp
Q_PROPERTY(bool enableAdvanced READ enableAdvanced
           WRITE setEnableAdvanced NOTIFY enableAdvancedChanged
           DESIGNABLE true)
Q_PROPERTY(int advancedValue READ advancedValue
           WRITE setAdvancedValue DESIGNABLE true)
```

**第 2 步** — 在构造函数中注册规则：

```cpp
m_policy->setVisibilityRule(
    "advancedValue",                          // 被控属性
    [this] { return m_enableAdvanced; },      // 谓词
    {"enableAdvanced"}                        // 依赖列表
);
```

**第 3 步** — 依赖属性变化时通知 policy：

```cpp
connect(this, &MyWidget::enableAdvancedChanged,
        m_policy, &DesignerPropertyPolicy::reevaluate);
```

**可用的规则类型**：

| 方法                | 属性编辑器中的效果       |
| ------------------- | ------------------------ |
| `setVisibilityRule` | 显示 / 隐藏属性          |
| `setEnabledRule`    | 可编辑 / 变灰            |
| `setResetRule`      | 显示 / 隐藏重置按钮      |
| `setAttributeRule`  | 将属性视为 Designer 属性 |
| `setValueOverride`  | 替换编辑器中显示的值     |

**重要**：依赖列表是必须的。没有它，依赖属性变化时规则不会重新求值，
属性编辑器也不会刷新。

### 仓库结构

```
designer-dynamic-props-wizard/
├── README.md                    ← 本文件
├── LICENSE                      ← MIT
├── install.sh                   ← Linux / macOS 安装脚本
├── install.bat                  ← Windows 安装脚本
└── designer-dynamic-props/      ← 向导本体（安装时复制此目录）
    ├── wizard.json
    ├── widget/
    │   ├── widget.h
    │   └── widget.cpp
    ├── plugin/
    │   ├── plugin.h
    │   └── plugin.cpp
    ├── lib/
    │   ├── designerdynamicproperties.h        ← 契约（仅 QtCore）
    │   ├── designerpropertypolicy.h/.cpp      ← 规则引擎（仅 QtCore）
    │   ├── dynamicpropertysheetwrapper.h/.cpp ← Designer 包装器
    │   └── dynamicpropertysheetwrapperfactory.h/.cpp
    ├── qmake/
    │   ├── all.pro
    │   ├── widget.pro
    │   └── plugin.pro
    ├── cmake5/
    │   └── *.CMakeLists.txt       ← 兼容 Qt 5
    └── cmake6/
        └── *.CMakeLists.txt       ← 仅 Qt 6
```

### 卸载

- **Windows**：`install.bat /uninstall`
- **Linux / macOS**：`./install.sh --uninstall`

或手动删除 Qt Creator 向导目录下的 `designer-dynamic-props/` 文件夹。
重启 Qt Creator。

### 许可证

MIT — 见 [LICENSE](LICENSE)。你可以自由使用、修改和再分发本向导及其
生成的任何项目，无任何限制。