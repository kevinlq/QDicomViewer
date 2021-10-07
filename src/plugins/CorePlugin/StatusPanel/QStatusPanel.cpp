#include "QStatusPanel.h"

#include "../CoreInclude.h"
#include "../coreconstants.h"

using namespace Core;
QStatusPanel::QStatusPanel(QWidget *parent)
    : QWidget(parent)
    , m_pLabelInfo(new QLabel(this))
    , m_pProgressBar(new QProgressBar(this))
{
    m_pLabelInfo->setStyleSheet("QLabel{color:#C8C8C8}");
    m_pLabelInfo->setText("Ready");
    m_pLabelInfo->setObjectName("label_status");

    m_pProgressBar->setFixedWidth(Constants::LIST_PANEL_WIDTH);
    m_pProgressBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    m_pProgressBar->setObjectName("progressbar_status");
    m_pProgressBar->setRange(0, 100);
    m_pProgressBar->setStyleSheet("QProgressBar#progressbar_status{text-align:center;background-color:rgb(30,30,30);color:white;border:2px rgb(145,145,145);}QProgressBar::chunk{background-color:rgb(170,170,170)}");
    m_pProgressBar->setValue(60);

    m_pProgressBar->setVisible(false);

    QHBoxLayout* pProgressLayout = new QHBoxLayout;
    pProgressLayout->addWidget(m_pProgressBar);
    pProgressLayout->setContentsMargins(2,4,2,4);

    QHBoxLayout* pVBoxLayout = new QHBoxLayout(this);

    pVBoxLayout->addSpacing(6);
    pVBoxLayout->addWidget(m_pLabelInfo);
    pVBoxLayout->addStretch();
    pVBoxLayout->addLayout(pProgressLayout);
    pVBoxLayout->addSpacing(6);
    pVBoxLayout->setContentsMargins(0,0,0,0);

    setLayout(pVBoxLayout);
}

void QStatusPanel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    QRect rc = this->rect();

    painter.fillRect(rc, Constants::STATUS_PANEL_BACK_COLOR);
}

void QStatusPanel::updateProgressValue(int nValue)
{
    m_pProgressBar->setValue(nValue);
}

void QStatusPanel::setProgressRange(int nMin, int nMax)
{
    m_pProgressBar->setRange(nMin, nMax);
}
