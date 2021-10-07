#include "QListPanelManagerPrivate.h"

#include "../CoreDataStruct.h"

QListPanelManagerPrivate::QListPanelManagerPrivate(QObject *parent)
    : QObject(parent)
{
    m_lsPanelContainer.clear();
}

QListPanelManagerPrivate::~QListPanelManagerPrivate()
{
    cleanListItem(-1);
}

void QListPanelManagerPrivate::cleanListItem(int nIndex)
{
    LS_PANELCONTAINER::iterator iter = m_lsPanelContainer.begin();

    if (nIndex == -1)
    {
        for(; iter != m_lsPanelContainer.end(); iter++)
        {
            ListItemData* pListItem = (ListItemData*)(*iter);

            if ( nullptr == pListItem)
            {
                continue;
            }

            m_lsPanelContainer.erase(iter);

            delete pListItem;
            pListItem = nullptr;
        }

        m_lsPanelContainer.clear();
    }
}
