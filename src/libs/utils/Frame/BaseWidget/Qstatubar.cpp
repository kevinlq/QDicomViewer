#include "Qstatubar.h"

#include <QLabel>
#include <QToolButton>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLatin1String>
#include <QCursor>
#include <QPainter>

const int L_MARGIN =10;
const int R_MARGIN = 50;
const int S_WIDGET = 20;

QStatubar::QStatubar(QWidget *parent) :
    QWidget(parent)
{
    this->initWidget();
}

QStatubar::~QStatubar()
{
}

void QStatubar::setStatueIcon(const QString &path)
{
    m_pLabelIcon->setPixmap(QPixmap(QString("%1").arg(path)));
}

void QStatubar::setAuthorName(const QString &name)
{
    m_pLabelAuthor->setText(name);
}

void QStatubar::setCompanyName(const QString &name)
{
    m_pLabelCompany->setText(name);
}

void QStatubar::initWidget()
{
    QWidget* pwidgetBack = new QWidget(this);
    pwidgetBack->setObjectName ("widget_bottom");

    m_pLabelAuthor = new QLabel("admin",pwidgetBack);
    m_pLabelCompany = new QLabel("kevinlq",pwidgetBack);
    m_pLabelIcon = new QLabel(this);

    m_pLayout = new QHBoxLayout(pwidgetBack);
    m_pLayout->addSpacing(2);
    m_pLayout->addWidget(m_pLabelIcon);
    m_pLayout->addWidget(m_pLabelAuthor);
    m_pLayout->addWidget(m_pLabelCompany);
    m_pLayout->addStretch(1);
    m_pLayout->setContentsMargins(L_MARGIN,0,R_MARGIN,0);
    m_pLayout->setSpacing(S_WIDGET);

    QVBoxLayout* pVLayout = new QVBoxLayout(this);
    pVLayout->addWidget(pwidgetBack);
    pVLayout->setContentsMargins(0,0,0,0);

    setLayout(pVLayout);

    setFixedHeight(30);
}
