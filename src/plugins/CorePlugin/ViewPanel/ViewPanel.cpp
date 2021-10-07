#include "ViewPanel.h"
#include "../CoreInclude.h"

#include "../DragProgress.h"
#include "../CoreDataStruct.h"
#include "../CoreUtil.h"
#include "DImageView.h"

using namespace Core;

ViewPanel::ViewPanel(QWidget *parent)
    : QWidget(parent)
    , m_nActivedView(0)
    , m_bSingleView(false)
{
    m_vtrpImageView.clear();
}

ViewPanel::~ViewPanel()
{
    clearAllImageView();
}

void ViewPanel::updateViewLayout()
{
    updateLayoutInfo();

    hideAllImagView();

    for (int i = 0; i < m_vtrLayoutInfo.size(); i++)
    {
        DImageView *pImageView = getImageView(m_vtrLayoutInfo[i].m_nViewID);

        if (Q_NULLPTR == pImageView)
        {
            pImageView = new(std::nothrow) DImageView(this);
            pImageView->setViewID(m_vtrLayoutInfo[i].m_nViewID);
            m_vtrpImageView.push_back(pImageView);

            connect(pImageView, &DImageView::signalEvent, this, &ViewPanel::onSignalEvent);
        }

        pImageView->setVisible(true);
        pImageView->setGeometry(m_vtrLayoutInfo[i].m_rcView);
    }

    // 设置激活窗口
    setImageViewActived(m_nActivedView, true);
}

void ViewPanel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    QRect rc = this->rect();

    rc.setWidth(rc.width());
    rc.setHeight(rc.height());

    painter.fillRect(rc, QColor(130,130,130));
}

void ViewPanel::resizeEvent(QResizeEvent *event)
{
    updateViewLayout();

    QWidget::resizeEvent(event);
}

void ViewPanel::onSignalEvent(int nEventID, const QVariant &param)
{
    if (EventActiveView == nEventID)
    {
        m_nActivedView = param.toInt();

        setAllImageViewActived(false);
        setImageViewActived(m_nActivedView, true);
    }
    else if (EventDoubleClickView == nEventID)
    {
        // 双击切换分格
        m_nActivedView = param.toInt();
        m_bSingleView = !m_bSingleView;

        updateViewLayout();
    }
}

DImageView *ViewPanel::getImageView(int nViewID)
{
    VTRP_IMAGEVIEW::iterator iter = m_vtrpImageView.begin();

    for (; iter != m_vtrpImageView.end(); iter++)
    {
        if (Q_NULLPTR != *iter && (*iter)->getViewID() == nViewID)
        {
            return *iter;
        }
    }

    return Q_NULLPTR;
}

bool ViewPanel::clearAllImageView()
{
    VTRP_IMAGEVIEW::iterator iter = m_vtrpImageView.begin();

    for (; iter != m_vtrpImageView.end(); )
    {
        DImageView *pImage = dynamic_cast<DImageView*>(*iter);

        iter = m_vtrpImageView.erase(iter);

        if (Q_NULLPTR != pImage)
        {
            delete pImage;
            pImage = Q_NULLPTR;
        }
    }

    return true;
}

void ViewPanel::hideAllImagView()
{
    VTRP_IMAGEVIEW::iterator iter = m_vtrpImageView.begin();

    for (; iter != m_vtrpImageView.end(); iter++)
    {
        DImageView *pImage = dynamic_cast<DImageView*>(*iter);

        if (Q_NULLPTR != pImage)
        {
            pImage->setVisible(false);
        }
    }
}

void ViewPanel::setAllImageViewActived(bool bActived)
{
    VTRP_IMAGEVIEW::iterator iter = m_vtrpImageView.begin();

    for (; iter != m_vtrpImageView.end(); iter++)
    {
        DImageView *pImage = dynamic_cast<DImageView*>(*iter);

        if (Q_NULLPTR != pImage)
        {
            pImage->setActived(bActived);
            pImage->update();
        }
    }
}

bool ViewPanel::setImageViewActived(int nViewID, bool bActived)
{
    DImageView *pImageView = getImageView(nViewID);

    if (Q_NULLPTR == pImageView)
    {
        return false;
    }

    pImageView->setActived(bActived);
    pImageView->update();

    return true;
}

void ViewPanel::updateLayoutInfo()
{
    m_vtrLayoutInfo.clear();

    int nWidth = this->width();
    int nHeight = this->height();

    if (m_bSingleView)
    {
        m_vtrLayoutInfo.push_back(DS_LayoutInfo(m_nActivedView, QRect(0, 0, nWidth, nHeight)));
    }
    else
    {
        // 默认四分格，后续可以通过配置文件扩展
        m_vtrLayoutInfo.push_back(DS_LayoutInfo(0, QRect(0, 0, nWidth*0.5, nHeight*0.5)));
        m_vtrLayoutInfo.push_back(DS_LayoutInfo(1, QRect(nWidth*0.5, 0, nWidth*0.5, nHeight*0.5)));
        m_vtrLayoutInfo.push_back(DS_LayoutInfo(2, QRect(0, nHeight*0.5,nWidth*0.5, nHeight*0.5)));
        m_vtrLayoutInfo.push_back(DS_LayoutInfo(3, QRect(nWidth*0.5, nHeight*0.5, nWidth*0.5, nHeight*0.5)));
    }
}
