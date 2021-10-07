#include "QListPanelManager.h"
#include "QListPanelManagerPrivate.h"

#include "../id.h"

using namespace Core;
//using namespace Core::Internal;

static QListPanelManager* m_pInstance = nullptr;
static QListPanelManagerPrivate* d = nullptr;

QListPanelManager *QListPanelManager::getInstance()
{
    return m_pInstance;
}

QList<ListItemData *> *QListPanelManager::getAllItems()
{
    return &(d->m_lsPanelContainer);
}

bool QListPanelManager::addItem(ListItemData *pData)
{
    if (nullptr == pData)
    {
        return false;
    }

    d->m_lsPanelContainer.append(pData);

    emit signalAddItem(*pData);

    return true;
}

QListPanelManager::QListPanelManager(QObject *parent)
    : QObject(parent)
{
    m_pInstance = this;
    d = new(std::nothrow) QListPanelManagerPrivate(this);
}
