#ifndef DICOMLISTVIEW_H
#define DICOMLISTVIEW_H

#include <QListView>

class ListItemData;

class DicomListView : public QListView
{
    Q_OBJECT
    Q_DISABLE_COPY(DicomListView)
public:
    DicomListView(QWidget* parent = nullptr);
    ~DicomListView();

protected:
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;

Q_SIGNALS:
    void signalPeriodActive(const QVariantMap &mapInfo);
    void signalAddDicom(const QVariantList &lsFiles);

private:
    bool getClickItemData(const QModelIndex &index, ListItemData &data);
private:
    QPoint m_dragStartPosition;
};

#endif // DICOMLISTVIEW_H
