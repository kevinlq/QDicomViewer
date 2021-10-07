#include "DragProgress.h"
#include "CoreInclude.h"

#include "coreconstants.h"
#include "CoreUtil.h"

namespace Core{

DragProgress::DragProgress()
{

}

bool DragProgress::dragEnterProc(QDragEnterEvent *pEvent, DS_DragFilter *pDragFilter)
{
    bool bValidFile = false;
    const QMimeData* pMimeData = pEvent->mimeData();

    if (pMimeData->hasFormat(QLatin1String(Constants::DROP_EXTEND_KEY)))
    {
        // 外部拖入文件进入
        if (pDragFilter->m_eFromFilterType == DRAG_EXTEND_FILE
                && (pDragFilter->m_eToFilterType == DRAG_LIST_PANEL || pDragFilter->m_eToFilterType == DRAG_VIEW_PANEL))
        {
            QList<QUrl> urls = pMimeData->urls();
            if (urls.isEmpty())
            {
                return bValidFile;
            }

            bValidFile = true;
            if (urls.size() == 1)
            {
                // 单个文件校验，不是dicom文件不处理
                bValidFile = CoreUtil::isValidFile(urls[0], true, QLatin1String(Constants::DROP_EXTEND_FILTER));
            }
        }
    }
    else if (pMimeData->hasFormat(QLatin1String(Constants::DROP_LIST_PANEL_KEY)))
    {
        // 程序内部窗口之间拖入元素进入
        if (pDragFilter->m_pSource != pEvent->source())
        {
            // 默认，单个窗口内部拖动不处理.
            bValidFile = true;
        }
    }

    return bValidFile;
}

bool DragProgress::dragMoveEventProc(QDragMoveEvent *pEvent, DS_DragFilter *pDragFilter)
{
    bool bValidFile = false;
    const QMimeData* pMimeData = pEvent->mimeData();

    if (pMimeData->hasFormat(QLatin1String(Constants::DROP_EXTEND_KEY)))
    {
        // 外部拖入文件进入后移动
        if (pDragFilter->m_eFromFilterType == DRAG_EXTEND_FILE
                && (pDragFilter->m_eToFilterType == DRAG_LIST_PANEL || pDragFilter->m_eToFilterType == DRAG_VIEW_PANEL))
        {
            bValidFile = true;
        }
    }
    else if (pMimeData->hasFormat(QLatin1String(Constants::DROP_LIST_PANEL_KEY)))
    {
        // 程序内部窗口之间拖入元素进入
        if (pDragFilter->m_pSource != pEvent->source())
        {
            // 默认，单个窗口内部拖动不处理.
            bValidFile = true;
        }
    }

    return bValidFile;
}

bool DragProgress::dropEventProc(QDropEvent *pEvent, DS_DragFilter *pDragFilter)
{
    bool bValidFile = false;
    const QMimeData* pMimeData = pEvent->mimeData();

    if (pMimeData->hasFormat(QLatin1String(Constants::DROP_EXTEND_KEY)))
    {
        pDragFilter->m_eFromFilterType = DRAG_EXTEND_FILE;
        QList<QUrl> urls = pMimeData->urls();

        bValidFile = CoreUtil::parseFileFromUrl(urls, pDragFilter->m_lsDragData);
    }
    else if (pMimeData->hasFormat(QLatin1String(Constants::DROP_LIST_PANEL_KEY)))
    {
        pDragFilter->m_eFromFilterType = DRAG_LIST_PANEL;

        QByteArray itemData = pMimeData->data(QLatin1String(Constants::DROP_LIST_PANEL_KEY));

        pDragFilter->m_lsDragData.clear();
        pDragFilter->m_lsDragData.push_back(itemData);

        bValidFile = true;
    }

    return bValidFile;
}

}
