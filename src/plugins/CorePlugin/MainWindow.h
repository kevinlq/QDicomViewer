#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <utils/appmainwindow.h>
#include "icore.h"
#include "icontext.h"

class ContentWidgetContainer;

namespace Core {
namespace Internal
{

class VersionDialog;
class MainWindow : public Utils::AppMainWindow
{
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();

    bool init(QString *errorMessage);
    void extensionsInitialized();
    void aboutToShutdown();

    IContext *contextObject(QWidget *widget);
    void addContextObject(IContext *context);
    void removeContextObject(IContext *context);

    IContext * currentContextObject() const;

    void updateAdditionalContexts(const Context &remove, const Context &add,
                                  ICore::ContextPriority priority);

    QStringList additionalAboutInformation() const;
    void appendAboutInformation(const QString &line);

    bool showWarningWithOptions(const QString &title,
                                const QString &text,
                                const QString &details,
                                Id settingsId,
                                QWidget *parent);

    void readSettings();
    void saveSettings();

private:
    void updateFocusWidget(QWidget *old, QWidget *now);

    void restoreWindowState();

    void registerDefaultContainers();
    void registerDefaultActions();

    void updateContextObject(const QList<IContext *> &context);
    void updateContext();

    void destroyVersionDialog();
    void aboutQDicomViewer();
    void aboutPlugins();

private Q_SLOTS:
    bool showOptionsDialog(Id page, QWidget *parent = Q_NULLPTR);

protected:
    virtual void closeEvent(QCloseEvent *event);

protected:
    ICore                   *m_coreImpl         = nullptr;
    QStringList              m_aboutInformation;
    ContentWidgetContainer  *m_pContentWidget   = nullptr;
    VersionDialog *m_pVersionDialog             = nullptr;

    QList<IContext *> m_activeContext;
    QMap<QWidget *, IContext *> m_contextWidgets;

    Context m_highPrioAdditionalContexts;
    Context m_lowPrioAdditionalContexts;

    QColor m_overrideColor;
};

}
}
#endif // MAINWINDOW_H
