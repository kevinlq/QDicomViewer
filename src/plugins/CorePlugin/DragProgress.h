#ifndef DRAGPROGRESS_H
#define DRAGPROGRESS_H

#include <QWidget>

#include "core_global.h"

namespace Core{

enum DragFilter
{
    DRAG_NULL = -1,
    DRAG_EXTEND_FILE,
    DRAG_LIST_PANEL,
    DRAG_VIEW_PANEL
};

struct DS_DragFilter
{
    DragFilter      m_eFromFilterType;
    DragFilter      m_eToFilterType;
    QObject*        m_pSource;
    QVariantList    m_lsDragData;

    DS_DragFilter()
        :m_eFromFilterType(DRAG_NULL)
        ,m_eToFilterType(DRAG_NULL)
        ,m_pSource(nullptr)
        ,m_lsDragData(QVariantList())
    {
        //
    }
};

class DS_DragFilter;

class CORE_EXPORT DragProgress
{
public:
    DragProgress();

    static bool dragEnterProc(QDragEnterEvent* pEvent, DS_DragFilter* pDragFilter);
    static bool dragMoveEventProc(QDragMoveEvent* pEvent, DS_DragFilter* pDragFilter);
    static bool dropEventProc(QDropEvent* pEvent, DS_DragFilter* pDragFilter);
};
}

#endif // DRAGPROGRESS_H
