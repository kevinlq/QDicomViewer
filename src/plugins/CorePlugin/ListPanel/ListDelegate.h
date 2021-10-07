#ifndef LISTDELEGATE_H
#define LISTDELEGATE_H

#include <QStyledItemDelegate>

class ListDelegatePrivate;
class ListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ListDelegate(QObject* parent = nullptr);
    ~ListDelegate();

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
    virtual bool helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index);

private:
    ListDelegatePrivate* m_pImpl;
};

#endif // LISTDELEGATE_H
