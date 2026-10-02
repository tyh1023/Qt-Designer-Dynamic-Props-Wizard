#ifndef DYNAMICPROPERTYSHEETWRAPPER_H
#define DYNAMICPROPERTYSHEETWRAPPER_H

#include <QtCore/QObject>
#include <QtDesigner/QDesignerPropertySheetExtension>

class QDesignerDynamicProperties;

/**
 * Property sheet extension wrapper that forwards selected queries to a
 * QDesignerDynamicProperties implementation on the host widget.
 *
 * This class is Designer-plugin side only. It depends on the Qt Designer
 * private extension API and must not be linked into a widget library.
 *
 * The wrapper is created by DynamicPropertySheetWrapperFactory. It wraps
 * the default property sheet provided by Qt Designer, and delegates any
 * query for which no dynamic rule has been registered.
 */
class DynamicPropertySheetWrapper : public QObject,
public QDesignerPropertySheetExtension
{
	Q_OBJECT
	Q_INTERFACES(QDesignerPropertySheetExtension)
	
public:
	/**
	 * @param host          The widget being wrapped. May be any QObject; if
	 *                      it implements QDesignerDynamicProperties, its
	 *                      rules are consulted.
	 * @param defaultSheet  The default property sheet returned by Qt
	 *                      Designer, or nullptr if unavailable.
	 * @param parent        QObject parent.
	 */
	DynamicPropertySheetWrapper(QObject *host,
								QDesignerPropertySheetExtension *defaultSheet,
								QObject *parent = nullptr);
	
	// ---- QDesignerPropertySheetExtension ----
	int     count() const override;
	int     indexOf(const QString &name) const override;
	QString propertyName(int index) const override;
	QString propertyGroup(int index) const override;
	void    setPropertyGroup(int index, const QString &group) override;
	bool    hasReset(int index) const override;
	bool    reset(int index) override;
	bool    isVisible(int index) const override;
	void    setVisible(int index, bool visible) override;
	bool    isAttribute(int index) const override;
	void    setAttribute(int index, bool attribute) override;
	QVariant property(int index) const override;
	void    setProperty(int index, const QVariant &value) override;
	bool    isChanged(int index) const override;
	void    setChanged(int index, bool changed) override;
	bool    isEnabled(int index) const override;
	
private slots:
	/**
	 * Connected to designerDynamicRulesChanged() on the host's rule
	 * notifier. Triggers a Designer property-editor refresh.
	 */
	void onRulesChanged();
	
private:
	QObject *m_host;
	QDesignerDynamicProperties *m_dynamic = nullptr;
	QDesignerPropertySheetExtension *m_defaultSheet = nullptr;
};

#endif // DYNAMICPROPERTYSHEETWRAPPER_H

