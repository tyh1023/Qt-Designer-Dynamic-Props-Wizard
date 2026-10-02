#include "%{WidgetClassName}.h"
#include "designerpropertypolicy.h"

%{WidgetClassName}::%{WidgetClassName}(%{WidgetBaseClass} *parent)
    : %{WidgetBaseClass}(parent)
{
    m_policy = new DesignerPropertyPolicy(this, this);

    // Register your dynamic property rules here.
    // See example/ExampleWidget.cpp for a working sample.

    // Whenever a property that a rule depends on changes, notify the policy:
    //
    //   connect(this, &%{WidgetClassName}::somePropertyChanged,
    //           m_policy, &DesignerPropertyPolicy::reevaluate);
}

// --- QDesignerDynamicProperties forwarding ---
// These are boilerplate. You normally do not need to change them.

std::optional<bool> %{WidgetClassName}::propertyVisible(const QString &n) const
{ return m_policy->propertyVisible(n); }

std::optional<bool> %{WidgetClassName}::propertyEnabled(const QString &n) const
{ return m_policy->propertyEnabled(n); }

std::optional<bool> %{WidgetClassName}::propertyResetable(const QString &n) const
{ return m_policy->propertyResetable(n); }

std::optional<bool> %{WidgetClassName}::propertyAttribute(const QString &n) const
{ return m_policy->propertyAttribute(n); }

QObject *%{WidgetClassName}::ruleChangeNotifier()
{ return m_policy; }
