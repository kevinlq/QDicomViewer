#ifndef QLISTPANELMANAGER_H
#define QLISTPANELMANAGER_H

#include <QObject>
class ListItemData;

#ifndef ListPanelMgr
#define ListPanelMgr Core::QListPanelManager::getInstance()
#endif

namespace Core {

namespace Internal {
class CorePlugin;
class MainWindow;
} // Internal

class QListPanelManager : public QObject
{
    Q_OBJECT
public:
    static QListPanelManager* getInstance();

Q_SIGNALS:
    void signalAddItem(const ListItemData &pData);

public:
    QList<ListItemData*>* getAllItems();
    bool addItem(ListItemData *pData);

public:

private:
    explicit QListPanelManager(QObject *parent = 0);
    friend class Core::Internal::CorePlugin; // initialization

};

}

#endif // QLISTPANELMANAGER_H
