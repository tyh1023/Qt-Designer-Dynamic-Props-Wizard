#include "dynamicpropertysheetwrapperfactory.h"
#include "dynamicpropertysheetwrapper.h"
#include "designerdynamicproperties.h"

#include <QtCore/QSet>
#include <QtCore/QGlobalStatic>
#include <QtDesigner/QDesignerPropertySheetExtension>
#include <QtDesigner/QExtensionManager>

// Guards against infinite recursion: while we are retrieving the default
// property sheet for an object, Qt Designer may call back into this factory.
// We track the objects we are currently processing and return nullptr for
// them so the default factory is used instead.
Q_GLOBAL_STATIC(QSet<QObject *>, recursionGuard)

DynamicPropertySheetWrapperFactory::DynamicPropertySheetWrapperFactory(
																	   QExtensionManager *parent)
: QExtensionFactory(parent)
{
}

QObject *DynamicPropertySheetWrapperFactory::createExtension(
															 QObject *object, const QString &iid, QObject *parent) const
{
	if (iid != Q_TYPEID(QDesignerPropertySheetExtension))
		return nullptr;
	
	// Only handle objects that implement the dynamic-properties contract.
	if (!qobject_cast<QDesignerDynamicProperties *>(object))
		return nullptr;
	
	// Break the recursion described above.
	if (recursionGuard->contains(object))
		return nullptr;
	
	QExtensionManager *manager = extensionManager();
	if (!manager)
		return nullptr;
	
	recursionGuard->insert(object);
	QObject *defaultExtension = manager->extension(object, iid);
	auto *defaultSheet =
	qobject_cast<QDesignerPropertySheetExtension *>(defaultExtension);
	recursionGuard->remove(object);
	
	if (!defaultSheet)
		return nullptr;
	
	return new DynamicPropertySheetWrapper(object, defaultSheet, parent);
}

