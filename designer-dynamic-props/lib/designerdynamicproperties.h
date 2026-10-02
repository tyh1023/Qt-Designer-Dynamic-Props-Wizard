#ifndef DESIGNERDYNAMICPROPERTIES_H
#define DESIGNERDYNAMICPROPERTIES_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <optional>

/**
 * Contract for widgets that want their Qt Designer property sheet to be
 * driven by runtime rules.
 *
 * Any QObject that implements this interface (via Q_INTERFACES) will be
 * picked up by the DynamicPropertySheetWrapperFactory installed by the
 * Designer plugin. The wrapper forwards visibility / enabled / reset /
 * attribute queries to the implementation.
 *
 * Return values:
 *   - std::nullopt : no rule for this property; fall back to the Designer
 *                    default behavior.
 *   - true / false : apply this result directly.
 *
 * propertyValueOverride() returns the value to display in the property
 * editor. Returning the input value unchanged means "no override".
 *
 * ruleChangeNotifier() must return a QObject that emits the signal
 *   designerDynamicRulesChanged(QStringList affectedProperties)
 * whenever a rule's result may have changed. The wrapper connects to this
 * signal and asks Designer to refresh the property sheet.
 *
 * This header intentionally depends only on QtCore so that it can live in
 * a widget library that has no Designer dependency.
 */
class QDesignerDynamicProperties
{
public:
	virtual ~QDesignerDynamicProperties() = default;
	
	/**
	 * Returns std::nullopt if no visibility rule is registered for the
	 * given property, or the rule's boolean result otherwise.
	 */
	virtual std::optional<bool> propertyVisible(const QString &name) const = 0;
	
	/**
	 * Returns std::nullopt if no enabled-state rule is registered for the
	 * given property, or the rule's boolean result otherwise.
	 * A false result greys out the property in the editor but keeps it
	 * visible.
	 */
	virtual std::optional<bool> propertyEnabled(const QString &name) const = 0;
	
	/**
	 * Returns std::nullopt if no reset rule is registered, or whether the
	 * property editor's "reset" action should be shown.
	 */
	virtual std::optional<bool> propertyResetable(const QString &name) const = 0;
	
	/**
	 * Returns std::nullopt if no attribute rule is registered, or whether
	 * the property should be treated as a Designer attribute.
	 */
	virtual std::optional<bool> propertyAttribute(const QString &name) const = 0;
	
	/**
	 * Allows the widget to substitute the value shown in the property
	 * editor. The default implementation returns the input unchanged.
	 */
	virtual QVariant propertyValueOverride(const QString &name,
										   const QVariant &current) const
	{
		Q_UNUSED(name);
		return current;
	}
	
	/**
	 * Returns the QObject whose signal
	 *   designerDynamicRulesChanged(QStringList)
	 * should be connected to by the wrapper. Returning nullptr disables
	 * automatic refresh.
	 */
	virtual QObject *ruleChangeNotifier() = 0;
};

#define QDesignerDynamicProperties_iid \
"org.qt-project.Qt.QDesigner.DynamicProperties/1.0"

Q_DECLARE_INTERFACE(QDesignerDynamicProperties, QDesignerDynamicProperties_iid)

#endif // DESIGNERDYNAMICPROPERTIES_H

