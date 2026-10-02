#include "%{WidgetClassName}Plugin.h"
#include "%{WidgetClassName}.h"
#include "dynamicpropertysheetwrapperfactory.h"

#include <QtDesigner/QDesignerFormEditorInterface>
#include <QtDesigner/QExtensionManager>
#include <QtDesigner/QDesignerPropertySheetExtension>

%{WidgetClassName}Plugin::%{WidgetClassName}Plugin(QObject *parent)
    : QObject(parent) {}

void %{WidgetClassName}Plugin::initialize(QDesignerFormEditorInterface *core)
{
    if (m_initialized) return;

    // Install the dynamic property sheet wrapper factory.
    // This factory is widget-agnostic: it matches any QObject that
    // implements QDesignerDynamicProperties, so it also powers any
    // additional widgets you add to this plugin later.
    core->extensionManager()->registerExtensions(
        new DynamicPropertySheetWrapperFactory(core->extensionManager()),
        Q_TYPEID(QDesignerPropertySheetExtension));

    m_initialized = true;
}

QWidget *%{WidgetClassName}Plugin::createWidget(QWidget *parent)
{
    return new %{WidgetClassName}(parent);
}

QString %{WidgetClassName}Plugin::domXml() const
{
    return QStringLiteral(R"(<ui language="c++">
 <widget class="%{WidgetClassName}" name="%{WidgetClassName}">
  <property name="geometry">
   <rect><x>0</x><y>0</y><width>200</width><height>100</height></rect>
  </property>
 </widget>
</ui>)");
}
