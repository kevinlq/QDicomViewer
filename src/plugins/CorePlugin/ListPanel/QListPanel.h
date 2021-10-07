#ifndef QLISTPANEL_H
#define QLISTPANEL_H

#include <QWidget>

class DicomListView;
class ListItemData;
class QStandardItemModel;

class QListPanel : public QWidget
{
    Q_OBJECT
public:
    explicit QListPanel(QWidget *parent = 0);
    ~QListPanel();

protected:
    virtual void paintEvent(QPaintEvent *event);

public Q_SLOTS:
    void onAddItem(const ListItemData &pData);

private:
    DicomListView*  m_pListView;
    QStandardItemModel *m_pModel;
};

#endif // QLISTPANEL_H
