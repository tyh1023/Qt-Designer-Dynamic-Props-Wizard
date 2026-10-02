#include "dynamicpropertysheetwrapper.h"
#include "designerdynamicproperties.h"

#include <QtDesigner/QDesignerFormWindowInterface>

DynamicPropertySheetWrapper::DynamicPropertySheetWrapper(
														 QObject *host,
														 QDesignerPropertySheetExtension *defaultSheet,
														 QObject *parent)
: QObject(parent)
, m_host(host)
, m_defaultSheet(defaultSheet)
{
	m_dynamic = qobject_cast<QDesignerDynamicProperties *>(host);
	
	if (m_dynamic) {
		QObject *notifier = m_dynamic->ruleChangeNotifier();
		if (notifier) {
			connect(notifier,
					SIGNAL(designerDynamicRulesChanged(QStringList)),
					this,
					SLOT(onRulesChanged()));
		}
	}
}

// ---- Pure forwarding to the default sheet ----

int DynamicPropertySheetWrapper::count() const
{
	return m_defaultSheet ? m_defaultSheet->count() : 0;
}

int DynamicPropertySheetWrapper::indexOf(const QString &name) const
{
	return m_defaultSheet ? m_defaultSheet->indexOf(name) : -1;
}

QString DynamicPropertySheetWrapper::propertyName(int index) const
{
	return m_defaultSheet ? m_defaultSheet->propertyName(index) : QString();
}

QString DynamicPropertySheetWrapper::propertyGroup(int index) const
{
	return m_defaultSheet ? m_defaultSheet->propertyGroup(index) : QString();
}

void DynamicPropertySheetWrapper::setPropertyGroup(int index, const QString &group)
{
	if (m_defaultSheet)
		m_defaultSheet->setPropertyGroup(index, group);
}

void DynamicPropertySheetWrapper::setVisible(int index, bool visible)
{
	if (m_defaultSheet)
		m_defaultSheet->setVisible(index, visible);
}

void DynamicPropertySheetWrapper::setAttribute(int index, bool attribute)
{
	if (m_defaultSheet)
		m_defaultSheet->setAttribute(index, attribute);
}

void DynamicPropertySheetWrapper::setProperty(int index, const QVariant &value)
{
	if (m_defaultSheet)
		m_defaultSheet->setProperty(index, value);
}

void DynamicPropertySheetWrapper::setChanged(int index, bool changed)
{
	if (m_defaultSheet)
		m_defaultSheet->setChanged(index, changed);
}

bool DynamicPropertySheetWrapper::reset(int index)
{
	return m_defaultSheet ? m_defaultSheet->reset(index) : false;
}

// ---- Queries that consult the dynamic rule engine ----

bool DynamicPropertySheetWrapper::hasReset(int index) const
{
	if (!m_defaultSheet)
		return false;
	
	if (m_dynamic) {
		const QString name = m_defaultSheet->propertyName(index);
		if (auto result = m_dynamic->propertyResetable(name); result.has_value())
			return *result;
	}
	return m_defaultSheet->hasReset(index);
}

bool DynamicPropertySheetWrapper::isVisible(int index) const
{
	if (!m_defaultSheet)
		return false;
	
	if (m_dynamic) {
		const QString name = m_defaultSheet->propertyName(index);
		if (auto result = m_dynamic->propertyVisible(name); result.has_value())
			return *result;
	}
	return m_defaultSheet->isVisible(index);
}

bool DynamicPropertySheetWrapper::isAttribute(int index) const
{
	if (!m_defaultSheet)
		return false;
	
	if (m_dynamic) {
		const QString name = m_defaultSheet->propertyName(index);
		if (auto result = m_dynamic->propertyAttribute(name); result.has_value())
			return *result;
	}
	return m_defaultSheet->isAttribute(index);
}

QVariant DynamicPropertySheetWrapper::property(int index) const
{
	if (!m_defaultSheet)
		return {};
	
	const QVariant current = m_defaultSheet->property(index);
	if (m_dynamic)
		return m_dynamic->propertyValueOverride(
												m_defaultSheet->propertyName(index), current);
	return current;
}

bool DynamicPropertySheetWrapper::isChanged(int index) const
{
	return m_defaultSheet ? m_defaultSheet->isChanged(index) : false;
}

bool DynamicPropertySheetWrapper::isEnabled(int index) const
{
	if (!m_defaultSheet)
		return true;
	
	if (m_dynamic) {
		const QString name = m_defaultSheet->propertyName(index);
		if (auto result = m_dynamic->propertyEnabled(name); result.has_value())
			return *result;
	}
	return m_defaultSheet->isEnabled(index);
}

// ---- Rule-change handler ----

void DynamicPropertySheetWrapper::onRulesChanged()
{
	if (auto *formWindow = QDesignerFormWindowInterface::findFormWindow(m_host))
		formWindow->emitSelectionChanged();
}

