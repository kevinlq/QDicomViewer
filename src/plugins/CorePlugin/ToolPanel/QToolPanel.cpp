#include "QToolPanel.h"
#include "../CoreInclude.h"

#include "QToolPanelManager.h"
#include "ToolButton.h"

#include "../coreconstants.h"
#include "ActionContainerPrivate.h"
#include "QToolPanelManager.h"


using namespace Core;
using namespace Core::Internal;

QToolPanel::QToolPanel(QWidget *parent)
    : QWidget(parent)
    , m_pHLayout(new QHBoxLayout)
{
    setLayout(m_pHLayout);
}

QToolPanel::~QToolPanel()
{
}

bool QToolPanel::addMenu(const Id &id)
{
    //
}

void QToolPanel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    QRect rc = this->rect();

    painter.fillRect(rc, Constants::BACKGROUND_COLOR);
}

void QToolPanel::updateToolPanel()
{
    QList<ActionContainer *> lsContainers = ToolPanelMgr->getAllActiontainers();

    if (lsContainers.isEmpty())
    {
        return;
    }

    for (int i = 0; i < lsContainers.size(); i++)
    {
        ActionContainerPrivate* pContainer = dynamic_cast<ActionContainerPrivate*>(lsContainers[i]);

        if (nullptr == pContainer)
        {
            continue;
        }

        QString strText = pContainer->id().suffixAfter(Constants::M_MENU_BASE);

        if (strText.isEmpty())
        {
            continue;
        }

        // 创建一级菜单
        ToolButton* pBn = new(std::nothrow) ToolButton(this);
        pBn->setMinimumSize(Constants::TOOL_BUTTON_WIDTH, Constants::TOOL_BUTTON_HEIGHT);
        pBn->setText(strText);
        pBn->setToolTip(tr(strText.toLocal8Bit()));

        if (nullptr == pContainer->menu())
        {
            continue;
        }

        for (int a = 0; a < pContainer->m_groups.size(); a++)
        {
            strText = pContainer->m_groups[a].id.suffixAfter(Constants::G_MENU_GROUP_BASE);
            pContainer->menu()->addAction(strText);
        }

        pBn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        pBn->setPopupMode(QToolButton::MenuButtonPopup);
        pBn->setMenu(pContainer->menu());

        m_pHLayout->addWidget(pBn);
    }

    m_pHLayout->addStretch();
    m_pHLayout->setSpacing(1);
    m_pHLayout->setContentsMargins(0,0,0,0);
}
