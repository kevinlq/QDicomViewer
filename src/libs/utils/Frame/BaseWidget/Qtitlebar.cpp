#include "Qtitlebar.h"

#include <QMouseEvent>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QPixmap>
#include <QHBoxLayout>
#include <QLatin1String>
#include <QCursor>
#include <QAction>
#include <QMenu>
#include <QMouseEvent>
#include <QRect>
#include <QVBoxLayout>
#include <QPainter>

#include <QDebug>

QTitlebar::QTitlebar(QWidget *parent) :
    QWidget(parent)
{
    this->initForm();
    this->initWidget();
    this->initConnect();
}

QTitlebar::~QTitlebar()
{
}

void QTitlebar::setTitleText(const QString &title)
{
    m_pLabelTitle->setText(title);
}

void QTitlebar::setTitleIcon(const QString &path)
{
    QPixmap pix(QString("%1").arg(path));
    m_pLabelIcon->setScaledContents(true);
    m_pLabelIcon->setPixmap(pix);
}

//样式初始化
void QTitlebar::initForm()
{
    //标题栏固定高度
    setFixedHeight(30);
    //setWindowFlags(Qt::FramelessWindowHint);
    //setAttribute(Qt::WA_TranslucentBackground);
}

void QTitlebar::initWidget()
{
    QWidget* pwidgetBackground = new QWidget(this);
    pwidgetBackground->setObjectName(QLatin1String("widget_title"));

    //程序图标
    m_pLabelIcon = new QLabel(pwidgetBackground);
    m_pLabelIcon->setText("");
    m_pLabelIcon->setObjectName(QLatin1String("lab_Ico"));

    //文本标签--标题
    m_pLabelTitle = new QLabel(pwidgetBackground);
    m_pLabelTitle->setText(tr("Application"));
    m_pLabelTitle->setObjectName(QLatin1String("lab_Title"));
    //按钮--菜单
    m_pBtnMenu = new QToolButton(pwidgetBackground);
    m_pBtnMenu->setObjectName("btnMenu");
    m_pBtnMenu->setToolTip(tr("menu"));

    //按钮--最小化
    m_pBtnMin = new QPushButton(pwidgetBackground);
    m_pBtnMin->setObjectName("btnMenu_Min");
    m_pBtnMin->setToolTip(tr("Min"));

    //按钮--最大化/还原
    m_pBtnMax = new QPushButton(pwidgetBackground);
    m_pBtnMax->setObjectName("btnMenu_Max");
    m_pBtnMax->setToolTip(tr("Max"));
    //按钮--关闭
    m_pBtnClose = new QPushButton(pwidgetBackground);
    m_pBtnClose->setObjectName("btnMenu_Close");
    m_pBtnClose->setToolTip(tr("close"));

    //水平布局
    m_pLayout = new QHBoxLayout(pwidgetBackground);
    m_pLayout->addSpacing(4);
    //添加部件
    m_pLayout->addWidget(m_pLabelIcon);
    m_pLayout->addSpacing(4);
    m_pLayout->addWidget(m_pLabelTitle);
    //添加伸缩项
    m_pLayout->addStretch();
    //添加部件
    m_pLayout->addWidget(m_pBtnMenu);
    m_pLayout->addWidget(m_pBtnMin);
    m_pLayout->addWidget(m_pBtnMax);
    m_pLayout->addWidget(m_pBtnClose);
    m_pLayout->addSpacing(2);
    m_pLayout->setSpacing(0);

    //设置Margin
    m_pLayout->setContentsMargins(0,0,0,0);

    QVBoxLayout* pVLayout = new QVBoxLayout(this);
    pVLayout->addWidget(pwidgetBackground);
    pVLayout->setContentsMargins(0,0,0,0);

    setLayout(pVLayout);
}

//信号和槽初始化
void QTitlebar::initConnect()
{
    connect(m_pBtnMin,&QPushButton::clicked,this,&QTitlebar::signalMin);
    connect(m_pBtnMax,&QPushButton::clicked, this, &QTitlebar::signalMax);
    connect(m_pBtnClose,&QPushButton::clicked,this, &QTitlebar::signalClose);
}
