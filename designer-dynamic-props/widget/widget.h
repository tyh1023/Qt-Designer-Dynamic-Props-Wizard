#pragma once

// %{WidgetClassName} - %{WidgetDisplayName}
//
// This widget implements QDesignerDynamicProperties so the Qt Designer
// property editor can show / hide / enable / disable properties at runtime.
//
// This library is built as a static library. No export macro is needed.
// See README.md for how to switch to a shared library.

#include <%{WidgetBaseClass}>
#include "designerdynamicproperties.h"

class DesignerPropertyPolicy;

class %{WidgetClassName} : public %{WidgetBaseClass}, public QDesignerDynamicProperties
{
    Q_OBJECT
    Q_INTERFACES(QDesignerDynamicProperties)

    // Add your own Q_PROPERTY declarations here.

public:
    explicit %{WidgetClassName}(%{WidgetBaseClass} *parent = nullptr);

    // --- QDesignerDynamicProperties interface ---
    std::optional<bool> propertyVisible  (const QString &n) const override;
    std::optional<bool> propertyEnabled  (const QString &n) const override;
    std::optional<bool> propertyResetable(const QString &n) const override;
    std::optional<bool> propertyAttribute(const QString &n) const override;
    QObject *ruleChangeNotifier() override;

private:
    DesignerPropertyPolicy *m_policy = nullptr;
};
