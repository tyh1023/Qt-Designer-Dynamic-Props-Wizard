#pragma once

#include <QtUiPlugin/customwidget.h>

// Qt Designer plugin for %{WidgetClassName}.
// Registers the widget and, in initialize(), installs the dynamic
// property sheet wrapper factory that powers runtime property rules.

class %{WidgetClassName}Plugin : public QObject,
                                public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetInterface")

public:
    explicit %{WidgetClassName}Plugin(QObject *parent = nullptr);

    bool isContainer() const override    { return false; }
    bool isInitialized() const override  { return m_initialized; }
    QIcon icon() const override          { return QIcon(); }
    QString domXml() const override;
    QString group() const override       { return QStringLiteral("@WidgetGroup@"); }
    QString includeFile() const override { return QStringLiteral("%{WidgetClassName}.h"); }
    QString name() const override        { return QStringLiteral("%{WidgetClassName}"); }
    QString toolTip() const override     { return QStringLiteral("@WidgetDisplayName@"); }
    QString whatsThis() const override   { return QString(); }
    QWidget *createWidget(QWidget *parent) override;
    void initialize(QDesignerFormEditorInterface *core) override;

private:
    bool m_initialized = false;
};