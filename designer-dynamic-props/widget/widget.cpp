#include "%{WidgetClassName}.h"
#include "designerpropertypolicy.h"

%{WidgetClassName}::%{WidgetClassName}(%{WidgetBaseClass} *parent)
    : %{WidgetBaseClass}(parent)
{
    m_policy = new DesignerPropertyPolicy(this, this);

    // Register your dynamic property rules here.
    // See example/ExampleWidget.cpp for a working sample.

    auto connectRuleTrigger = [this](auto signal, const char *propName) {
        connect(this, signal, m_policy, [this, propName]{
            m_policy->reevaluate(QString::fromLatin1(propName));
        });
    };

    // Whenever a property that a rule depends on changes, notify the policy:
    //
    //   connectRuleTrigger(&%{WidgetClassName}::somePropertyChanged,
    //          "someProperty");
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
