#ifndef QCOREPLUGIN_H
#define QCOREPLUGIN_H

#include <extensionsystem/iplugin.h>

namespace Core {

namespace Internal {

class MainWindow;

class CorePlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.kevinlq.QDVPlugin" FILE "Core.json")
public:
    CorePlugin();
    virtual~CorePlugin();

    bool initialize(const QStringList &arguments, QString *errorMessage = 0);
    void extensionsInitialized();
    bool delayedInitialize();
    ShutdownFlag aboutToShutdown();
    QObject *remoteCommand(const QStringList & /* options */,
                           const QString &workingDirectory,
                           const QStringList &args);
public slots:
    void fileOpenRequest(const QString&);
private:
    MainWindow *m_pMainWindow;
};

} // namespace Internal
} // namespace Core

#endif // QCOREPLUGIN_H
