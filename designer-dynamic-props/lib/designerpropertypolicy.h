#ifndef DESIGNERPROPERTYPOLICY_H
#define DESIGNERPROPERTYPOLICY_H

#include "designerdynamicproperties.h"

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QSet>
#include <QtCore/QStringList>
#include <functional>

/**
 * Rule engine for QDesignerDynamicProperties.
 *
 * A widget creates one DesignerPropertyPolicy, registers rules with it,
 * and forwards the QDesignerDynamicProperties interface calls to it.
 * Rules are small predicates (std::function<bool()>) paired with a list of
 * dependency property names. When a dependency changes, the widget calls
 * reevaluate() to notify the policy, which in turn emits
 * designerDynamicRulesChanged() for the wrapper to react to.
 *
 * Example:
 *
 *   m_policy = new DesignerPropertyPolicy(this, this);
 *
 *   m_policy->setVisibilityRule(
 *       "advancedValue",
 *       [this] { return m_enableAdvanced; },
 *       {"enableAdvanced"}
 *   );
 *
 *   connect(this, &MyWidget::enableAdvancedChanged,
 *           m_policy, &DesignerPropertyPolicy::reevaluate);
 */
class DesignerPropertyPolicy : public QObject, public QDesignerDynamicProperties
{
	Q_OBJECT
	Q_INTERFACES(QDesignerDynamicProperties)
	
public:
	using Predicate = std::function<bool()>;
	using ValueMapper = std::function<QVariant(const QVariant &)>;
	
	explicit DesignerPropertyPolicy(QObject *host = nullptr,
									QObject *parent = nullptr);
	
	// ---- Rule registration ----
	
	/**
	 * Registers a visibility rule.
	 * @param property  Name of the property whose visibility is controlled.
	 * @param predicate Returns true to show the property, false to hide it.
	 * @param dependsOn Names of properties that trigger re-evaluation.
	 */
	void setVisibilityRule(const QString &property,
						   Predicate predicate,
						   const QStringList &dependsOn = {});
	
	/**
	 * Registers an enabled-state rule.
	 * A false predicate result greys out the property in the editor.
	 */
	void setEnabledRule(const QString &property,
						Predicate predicate,
						const QStringList &dependsOn = {});
	
	/**
	 * Registers a reset-button rule.
	 * A false predicate result hides the reset action in the editor.
	 */
	void setResetRule(const QString &property,
					  Predicate predicate,
					  const QStringList &dependsOn = {});
	
	/**
	 * Registers an attribute rule.
	 */
	void setAttributeRule(const QString &property,
						  Predicate predicate,
						  const QStringList &dependsOn = {});
	
	/**
	 * Registers a value override. The mapper transforms the value shown in
	 * the property editor. Return the input unchanged to disable the
	 * override for this call.
	 */
	void setValueOverride(const QString &property,
						  ValueMapper mapper,
						  const QStringList &dependsOn = {});
	
	// ---- Rule removal ----
	
	/** Removes all rules registered for the given property. */
	void clearRule(const QString &property);
	
	/** Removes all rules. */
	void clearAllRules();
	
	// ---- Notification ----
	
	/**
	 * Called by the owning widget after a property whose change may affect
	 * any rule has been updated. Only rules that declared a dependency on
	 * changedProperty are re-evaluated.
	 */
	void reevaluate(const QString &changedProperty);
	
	// ---- QDesignerDynamicProperties interface ----
	std::optional<bool> propertyVisible   (const QString &name) const override;
	std::optional<bool> propertyEnabled   (const QString &name) const override;
	std::optional<bool> propertyResetable (const QString &name) const override;
	std::optional<bool> propertyAttribute (const QString &name) const override;
	QVariant propertyValueOverride(const QString &name,
								   const QVariant &current) const override;
	QObject *ruleChangeNotifier() override;
	
	signals:
	/**
	 * Emitted after reevaluate() determines which properties may have
	 * changed. Receivers (the Designer wrapper) use this to refresh the
	 * property editor.
	 */
	void designerDynamicRulesChanged(const QStringList &affectedProperties);
	
private:
	struct Rule {
		Predicate predicate;
		QStringList dependsOn;
	};
	
	void registerRule(QHash<QString, Rule> &table,
					  const QString &property,
					  Predicate predicate,
					  const QStringList &dependsOn);
	
	void registerDependencies(const QString &property,
							  const QStringList &dependsOn);
	
	static std::optional<bool> evaluate(const QHash<QString, Rule> &table,
										const QString &property);
	
	QHash<QString, Rule> m_visibilityRules;
	QHash<QString, Rule> m_enabledRules;
	QHash<QString, Rule> m_resetRules;
	QHash<QString, Rule> m_attributeRules;
	
	struct ValueOverride {
		ValueMapper mapper;
		QStringList dependsOn;
	};
	QHash<QString, ValueOverride> m_valueOverrides;
	
	// Reverse dependency map: changed property -> set of affected properties.
	QHash<QString, QSet<QString>> m_reverseDependencies;
};

#endif // DESIGNERPROPERTYPOLICY_H

