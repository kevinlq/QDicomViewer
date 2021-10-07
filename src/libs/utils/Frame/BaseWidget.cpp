#include "BaseWidget.h"

#include "BaseWidget/Qtitlebar.h"
#include "BaseWidget/Qstatubar.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QMouseEvent>

#include <QDebug>

namespace Utils{

BaseWidget::BaseWidget(bool bTitle, bool bStatus, QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint)
    , m_pTitleBar(new QTitlebar(this))
    , m_pStatusBar(new QStatubar(this))
    , m_pContentWidget(new QWidget(this))
    , m_pMainLayout(new QVBoxLayout(this))
{
    m_pMainLayout->addWidget(m_pTitleBar);
    m_pMainLayout->addWidget(m_pContentWidget);
    m_pMainLayout->addWidget(m_pStatusBar);
    m_pMainLayout->setContentsMargins(0,0,0,0);

    setLayout(m_pMainLayout);

    //_pContentWidget->setObjectName(QLatin1String("widgetMain"));

    m_pTitleBar->setVisible(bTitle);
    m_pStatusBar->setVisible(bStatus);
}

BaseWidget::~BaseWidget()
{
}

void BaseWidget::setAppTitleIcon(const QString &strPath)
{
    m_pTitleBar->setTitleIcon(strPath);
}

void BaseWidget::setAppTitleInfo(const QString &strInfo)
{
    m_pTitleBar->setTitleText(strInfo);
}

void BaseWidget::setContentWidget(QWidget *p)
{
    m_pContentWidget = p;
}

}
