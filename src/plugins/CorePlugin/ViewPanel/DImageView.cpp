#include "DImageView.h"
#include "../CoreInclude.h"

#include "../DragProgress.h"
#include "../CoreDataStruct.h"
#include "../CoreUtil.h"

using namespace Core;

#define NORMAL_COLOR QColor(22,25,26)
#define ACTIVE_COLOR QColor(113,175,113)
#define HOVER_COLOR QColor(255,255,0)

DImageView::DImageView(QWidget *parent)
    : QWidget(parent)
    , m_nViewID(-1)
    , m_bActived(false)
    , m_bHovered(false)
{
    setAcceptDrops(true);
}

DImageView::~DImageView()
{
}

int DImageView::getViewID() const
{
    return m_nViewID;
}

void DImageView::setViewID(int nID)
{
    m_nViewID =nID;
}

void DImageView::setActived(bool bActive)
{
    m_bActived = bActive;
}

void DImageView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    QRect rcView = this->rect();
    QRect rc = rcView;

    rc.setWidth(rc.width() - 2);
    rc.setHeight(rc.height() - 2);

    rc.moveTopLeft(QPoint((rcView.width() - rc.width()) /2, (rcView.height() - rc.height())/2));

    painter.fillRect(rc, QColor(0,0,0));

    QColor borderColor(NORMAL_COLOR);
    if (m_bActived)
    {
        borderColor = ACTIVE_COLOR;
    }

    painter.save();
    painter.setPen(borderColor);
    painter.drawRect(rc);

    painter.restore();
}

void DImageView::dragEnterEvent(QDragEnterEvent *event)
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

void DImageView::dragMoveEvent(QDragMoveEvent *event)
{
    DS_DragFilter dragFilter;
    dragFilter.m_eFromFilterType = DRAG_EXTEND_FILE;
    dragFilter.m_eToFilterType = DRAG_VIEW_PANEL;
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

void DImageView::dropEvent(QDropEvent *event)
{
    DS_DragFilter dragFilter;

    bool bValidFile = DragProgress::dropEventProc(event, &dragFilter);

    if (bValidFile && !dragFilter.m_lsDragData.isEmpty())
    {
        if (dragFilter.m_eFromFilterType == DRAG_EXTEND_FILE)
        {
            // 通知外部加载dicom文件
           //emit signalAddDicom(dragFilter.m_lsDragData);

            qDebug() << "#### file";
        }
        else if (dragFilter.m_eFromFilterType == DRAG_LIST_PANEL)
        {
            QByteArray baData = dragFilter.m_lsDragData[0].toByteArray();
            ListItemData* pItemData = CoreUtil::listItemDataFromBuffer(baData);
            delete pItemData;
        }

        event->acceptProposedAction();
    }
    else
    {
        event->ignore();
    }
}

void DImageView::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    m_bActived = true;

    // 通知外部刷新其它状态
    emit signalEvent(EventActiveView, QVariant(m_nViewID));

    QWidget::mousePressEvent(event);
}

void DImageView::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit signalEvent(EventDoubleClickView, QVariant(m_nViewID));

    QWidget::mouseDoubleClickEvent(event);
}

void DImageView::leaveEvent(QEvent *event)
{
    m_bHovered = false;
    QWidget::leaveEvent(event);
}

void DImageView::enterEvent(QEvent *event)
{
    m_bHovered = true;
    QWidget::enterEvent(event);
}

