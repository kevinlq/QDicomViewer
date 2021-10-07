#ifndef QLISTPANELMANAGERPRIVATE_H
#define QLISTPANELMANAGERPRIVATE_H

#include <QObject>

class ListItemData;
class QListPanelManagerPrivate : public QObject
{
    Q_OBJECT
public:
    explicit QListPanelManagerPrivate(QObject *parent = 0);
    ~QListPanelManagerPrivate();

    void cleanListItem(int nIndex);

public:
    typedef QList<ListItemData*> LS_PANELCONTAINER;
    LS_PANELCONTAINER m_lsPanelContainer;
};

#endif // QLISTPANELMANAGERPRIVATE_H
