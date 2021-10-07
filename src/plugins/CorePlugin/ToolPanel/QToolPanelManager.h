#ifndef QTOOLPANELMANAGER_H
#define QTOOLPANELMANAGER_H

#include <QObject>

#include "../id.h"

#ifndef ToolPanelMgr
#define ToolPanelMgr Core::QToolPanelManager::getInstance()
#endif

namespace Core {

namespace Internal {
class QCorePlugin;
class QRMainWindow;
} // Internal

class ActionContainer;

class QToolPanelManager : public QObject
{
    Q_OBJECT
public:
    ~QToolPanelManager();
    static QToolPanelManager* getInstance();

    // 创建工具栏菜单
    static ActionContainer* createToolMenuPanel(Id id);

    // 创建菜单项
    static ActionContainer* createToolMenu(Id id);

    static ActionContainer *actionContainer(Id id);

    static QList<Id> getALLToolMenus();
    static QList<ActionContainer*> getAllActiontainers();

Q_SIGNALS:
    void signalUpdateToolPanel();

private:
    explicit QToolPanelManager(QObject *parent = nullptr);
    friend class Core::Internal::QCorePlugin; // initialization
};

}
#endif // QTOOLPANELMANAGER_H
