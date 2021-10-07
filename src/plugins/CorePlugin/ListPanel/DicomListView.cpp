#include "DicomListView.h"
#include "../CoreInclude.h"

#include "../DragProgress.h"
#include "../CoreDataStruct.h"
#include "../coreconstants.h"
#include "../CoreUtil.h"

using namespace Core;

DicomListView::DicomListView(QWidget *parent)
    : QListView(parent)
{
    setStyleSheet("QListView{background-color:rgba(100,100,100, 255)}");
}

DicomListView::~DicomListView()
{
}

void DicomListView::mouseMoveEvent(QMouseEvent *event)
{
    if(!(event->buttons() & Qt::LeftButton))
        return;
    if((event->pos() -m_dragStartPosition).manhattanLength() < QApplication::startDragDistance())
        return;

    QModelIndex index = indexAt(event->pos());

    QRect rc = visualRect(index);

    ListItemData data;
    if ( !getClickItemData(index, data))
    {
        return;
    }

    if (data.m_bTitleInfo || data.m_baThumbnailBuffer.isEmpty())
    {
        return;
    }

    QImage image = CoreUtil::imageFromBuffer(data.m_baThumbnailBuffer);
    QPixmap pixmap = QPixmap::fromImage(image);

    QByteArray itemData = CoreUtil::listItemDataToBuffer(&data);

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(QLatin1String(Constants::DROP_LIST_PANEL_KEY), itemData);

    QDrag *drag = new QDrag(this);

    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
    drag->setHotSpot(event->pos() - rc.topLeft());

    // this function not work,this is Qt bug.see https://bugreports.qt.io/browse/QTBUG-20835
    //https://stackoverflow.com/questions/12617686/how-override-cursor-when-dragging-out-the-app
    //QCursor cursor(Qt::OpenHandCursor);
    //drag->setDragCursor(cursor.pixmap(), Qt::CopyAction);

    if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction)
    {
    }
    else
    {
    }
}

void DicomListView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
    {
        return;
    }

    m_dragStartPosition = event->pos();
}

void DicomListView::mouseReleaseEvent(QMouseEvent *event)
{
    QModelIndex index = indexAt(event->pos());

    ListItemData itemData;
    if ( !getClickItemData(index, itemData))
    {
        return;
    }

    QVariantMap map;
    map["nPeriodID"] = "periodID";

    emit signalPeriodActive(map);
}

void DicomListView::dragEnterEvent(QDragEnterEvent *event)
{
    DS_DragFilter dragFilter;
    dragFilter.m_eFromFilterType = DRAG_EXTEND_FILE;
    dragFilter.m_eToFilterType = DRAG_LIST_PANEL;
    dragFilter.m_pSource = this;

    bool bValidFile = DragProgress::dragEnterProc(event, &dragFilter);

    if (bValidFile)
    {
        event->acceptProposedAction();
    }
    else
    {
        event->ignore();
    }
}

void DicomListView::dragMoveEvent(QDragMoveEvent *event)
{
    DS_DragFilter dragFilter;
    dragFilter.m_eFromFilterType = DRAG_EXTEND_FILE;
    dragFilter.m_eToFilterType = DRAG_LIST_PANEL;
    dragFilter.m_pSource = this;

    bool bValidFile = DragProgress::dragMoveEventProc(event, &dragFilter);


    if (bValidFile)
    {
        event->acceptProposedAction();
    }
    else
    {
        event->ignore();
    }
}

void DicomListView::dropEvent(QDropEvent *event)
{
    DS_DragFilter dragFilter;
    dragFilter.m_eFromFilterType = DRAG_EXTEND_FILE;
    dragFilter.m_eToFilterType = DRAG_LIST_PANEL;
    dragFilter.m_pSource = this;

    bool bValidFile = DragProgress::dropEventProc(event, &dragFilter);

    if (bValidFile)
    {
        if (dragFilter.m_lsDragData.isEmpty())
        {
            // 通知外部加载dicom文件
            emit signalAddDicom(dragFilter.m_lsDragData);
        }

        event->acceptProposedAction();
    }
    else
    {
        event->ignore();
    }
}

bool DicomListView::getClickItemData(const QModelIndex &index, ListItemData &data)
{
    if (!index.isValid())
    {
        return false;
    }

    QVariant variant = index.data(Qt::UserRole+1);
    data = variant.value<ListItemData>();

    return true;
}
