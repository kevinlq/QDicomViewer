#include "QToolPanelManager.h"
#include "QToolPanelManagerPrivate.h"

#include "ActionContainerPrivate.h"

#include <QHash>

using namespace Core;
using namespace Core::Internal;

namespace {
    enum { warnAboutFindFailures = 0 };
}
static QToolPanelManager* m_pInstance = nullptr;
static QToolPanelManagerPrivate* d = nullptr;

QToolPanelManager::QToolPanelManager(QObject *parent) : QObject(parent)
{
    m_pInstance = this;

    d = new(std::nothrow) QToolPanelManagerPrivate;
}

QToolPanelManager::~QToolPanelManager()
{
}

QToolPanelManager *QToolPanelManager::getInstance()
{
    return m_pInstance;
}

ActionContainer *QToolPanelManager::createToolMenuPanel(Id id)
{
    const QToolPanelManagerPrivate::IdContainerMap::const_iterator it = d->m_idContainerMap.constFind(id);
    if (it !=  d->m_idContainerMap.constEnd())
    {
        return it.value();
    }

    ToolMenuPanelContainer* pMenuPanel = new ToolMenuPanelContainer(id);

    d->m_idContainerMap.insert(id, pMenuPanel);
    connect(pMenuPanel, &QObject::destroyed, d, &QToolPanelManagerPrivate::containerDestroyed);

    return pMenuPanel;
}

ActionContainer *QToolPanelManager::createToolMenu(Id id)
{
    const QToolPanelManagerPrivate::IdContainerMap::const_iterator it = d->m_idContainerMap.constFind(id);
    if (it !=  d->m_idContainerMap.constEnd())
    {
        return it.value();
    }

    ToolMenuActionContainer* pMenuC = new ToolMenuActionContainer(id);

    d->m_idContainerMap.insert(id, pMenuC);
    connect(pMenuC, &QObject::destroyed, d, &QToolPanelManagerPrivate::containerDestroyed);

    return pMenuC;
}

ActionContainer *QToolPanelManager::actionContainer(Id id)
{
    const QToolPanelManagerPrivate::IdContainerMap::const_iterator it = d->m_idContainerMap.constFind(id);
    if (it == d->m_idContainerMap.constEnd())
    {
        if (warnAboutFindFailures)
            qWarning() << "ActionManagerPrivate::actionContainer(): failed to find :"
                       << id.name();
        return nullptr;
    }

    return it.value();
}

QList<Id> QToolPanelManager::getALLToolMenus()
{
    return d->m_idContainerMap.keys();
}

QList<ActionContainer *> QToolPanelManager::getAllActiontainers()
{
    return d->m_idContainerMap.values();
}
