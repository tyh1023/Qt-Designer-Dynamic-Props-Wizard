#include "designerpropertypolicy.h"

DesignerPropertyPolicy::DesignerPropertyPolicy(QObject *host, QObject *parent)
: QObject(parent)
{
	// 'host' is accepted for API symmetry with other policy objects and for
	// potential future use (e.g. automatic signal wiring). It is not used
	// here.
	Q_UNUSED(host);
}

void DesignerPropertyPolicy::registerDependencies(const QString &property,
												  const QStringList &dependsOn)
{
	for (const QString &dependency : dependsOn)
		m_reverseDependencies[dependency].insert(property);
}

void DesignerPropertyPolicy::registerRule(QHash<QString, Rule> &table,
										  const QString &property,
										  Predicate predicate,
										  const QStringList &dependsOn)
{
	table.insert(property, {std::move(predicate), dependsOn});
	registerDependencies(property, dependsOn);
}

void DesignerPropertyPolicy::setVisibilityRule(const QString &property,
											   Predicate predicate,
											   const QStringList &dependsOn)
{
	registerRule(m_visibilityRules, property, std::move(predicate), dependsOn);
}

void DesignerPropertyPolicy::setEnabledRule(const QString &property,
											Predicate predicate,
											const QStringList &dependsOn)
{
	registerRule(m_enabledRules, property, std::move(predicate), dependsOn);
}

void DesignerPropertyPolicy::setResetRule(const QString &property,
										  Predicate predicate,
										  const QStringList &dependsOn)
{
	registerRule(m_resetRules, property, std::move(predicate), dependsOn);
}

void DesignerPropertyPolicy::setAttributeRule(const QString &property,
											  Predicate predicate,
											  const QStringList &dependsOn)
{
	registerRule(m_attributeRules, property, std::move(predicate), dependsOn);
}

void DesignerPropertyPolicy::setValueOverride(const QString &property,
											  ValueMapper mapper,
											  const QStringList &dependsOn)
{
	m_valueOverrides.insert(property, {std::move(mapper), dependsOn});
	registerDependencies(property, dependsOn);
}

void DesignerPropertyPolicy::clearRule(const QString &property)
{
	m_visibilityRules.remove(property);
	m_enabledRules.remove(property);
	m_resetRules.remove(property);
	m_attributeRules.remove(property);
	m_valueOverrides.remove(property);
	// The reverse-dependency map may retain stale entries. This is
	// harmless: reevaluate() would simply emit a notification for a
	// property that no longer has a rule.
}

void DesignerPropertyPolicy::clearAllRules()
{
	m_visibilityRules.clear();
	m_enabledRules.clear();
	m_resetRules.clear();
	m_attributeRules.clear();
	m_valueOverrides.clear();
	m_reverseDependencies.clear();
}

void DesignerPropertyPolicy::reevaluate(const QString &changedProperty)
{
	QStringList affected;
	affected << changedProperty;                             // may have its own rules
	affected << m_reverseDependencies.value(changedProperty).values();
	
	if (!affected.isEmpty())
		emit designerDynamicRulesChanged(affected);
}

std::optional<bool> DesignerPropertyPolicy::evaluate(
													 const QHash<QString, Rule> &table, const QString &property)
{
	auto it = table.constFind(property);
	if (it == table.constEnd())
		return std::nullopt;
	return it->predicate();
}

std::optional<bool> DesignerPropertyPolicy::propertyVisible(
															const QString &name) const
{
	return evaluate(m_visibilityRules, name);
}

std::optional<bool> DesignerPropertyPolicy::propertyEnabled(
															const QString &name) const
{
	return evaluate(m_enabledRules, name);
}

std::optional<bool> DesignerPropertyPolicy::propertyResetable(
															  const QString &name) const
{
	return evaluate(m_resetRules, name);
}

std::optional<bool> DesignerPropertyPolicy::propertyAttribute(
															  const QString &name) const
{
	return evaluate(m_attributeRules, name);
}

QVariant DesignerPropertyPolicy::propertyValueOverride(
													   const QString &name, const QVariant &current) const
{
	auto it = m_valueOverrides.constFind(name);
	return it == m_valueOverrides.constEnd() ? current : it->mapper(current);
}

QObject *DesignerPropertyPolicy::ruleChangeNotifier()
{
	return this;
}

