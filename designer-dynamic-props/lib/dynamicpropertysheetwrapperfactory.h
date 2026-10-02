#ifndef DYNAMICPROPERTYSHEETWRAPPERFACTORY_H
#define DYNAMICPROPERTYSHEETWRAPPERFACTORY_H

#include <QtDesigner/QExtensionFactory>

/**
 * Extension factory that produces DynamicPropertySheetWrapper instances
 * for any QObject implementing QDesignerDynamicProperties.
 *
 * The factory is intentionally widget-agnostic: it does not know about any
 * concrete widget class. Registering it once with the Designer extension
 * manager enables dynamic property rules for every widget in the plugin
 * that implements the contract.
 */
class DynamicPropertySheetWrapperFactory : public QExtensionFactory
{
	Q_OBJECT
	
public:
	explicit DynamicPropertySheetWrapperFactory(QExtensionManager *parent = nullptr);
	
protected:
	QObject *createExtension(QObject *object,
							 const QString &iid,
							 QObject *parent) const override;
};

#endif // DYNAMICPROPERTYSHEETWRAPPERFACTORY_H

