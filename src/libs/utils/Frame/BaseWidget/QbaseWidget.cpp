#include "QbaseWidget.h"
#include "Qtitlebar.h"
#include "Qstatubar.h"

#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPushButton>
#include <QPainter>
#include <QTextStream>
#include <QTranslator>
#include <QApplication>
#include <QTextCodec>
#include <QBitmap>
#include <QPainter>
#include <qmath.h>

QBaseWidget::QBaseWidget(bool bShowTitleBar, QWidget *pContentWidget,
                         bool bShowStatuBar, QWidget *parent)
    :QFrame(parent),m_pContentWidget(pContentWidget)
{
    this->initForm();

    m_pTitleBar = new QTitlebar(this);
    pContentWidget = new QWidget(this);
    m_pStatuBar = new QStatubar(this);

    m_pMainLayout = new QVBoxLayout(this);
    m_pMainLayout->addWidget(m_pTitleBar);
    m_pMainLayout->addWidget(m_pContentWidget);
    m_pMainLayout->addWidget(m_pStatuBar);
    m_pMainLayout->setContentsMargins(0,0,0,0);

    m_pTitleBar->setVisible(bShowTitleBar);
    m_pStatuBar->setVisible(bShowStatuBar);

    this->initConnect();
    this->initStyle();
}

void QBaseWidget::setWidgetBtnShow(bool bMinBtnShow, bool bMenuBtnShw,
                                   bool bMaxBtnShow, bool bCloseBtnShow)
{
    m_pTitleBar->m_pBtnMenu->setVisible(bMenuBtnShw);
    m_pTitleBar->m_pBtnMin->setVisible(bMinBtnShow);
    m_pTitleBar->m_pBtnMax->setVisible(bMaxBtnShow);
    m_pTitleBar->m_pBtnClose->setVisible(bCloseBtnShow);
}

void QBaseWidget::setWidgetTitleText(const QString &str)
{
    m_pTitleBar->setTitleText(str);
    this->setWindowTitle(str);
}

void QBaseWidget::setWidgetIcon(const QString &path)
{
    m_pTitleBar->setTitleIcon(path);
}

void QBaseWidget::setStatueIcon(const QString &path)
{
    m_pStatuBar->setStatueIcon(path);
}

void QBaseWidget::setAuthorName(const QString &name)
{
    m_pStatuBar->setAuthorName(name);
}

void QBaseWidget::setCompanyName(const QString &name)
{
    m_pStatuBar->setCompanyName(name);
}

bool QBaseWidget::getClosing()
{
    return m_bIsClose;
}

bool QBaseWidget::getBoolMaxWin()
{
    return m_bMaxWin;
}

void QBaseWidget::initForm()
{
    //隐藏标题栏
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground,true);
    //默认鼠标没有按下
    m_bIsPressed = false;

    m_bIsClose = false;
    m_bMaxWin = false;
}

void QBaseWidget::initConnect()
{
    connect(m_pTitleBar,SIGNAL(signalMin()),
            this,SLOT(slotShowMin()));
    connect(m_pTitleBar,SIGNAL(signalMax()),
            this,SLOT(slotShowMax()));
    connect(m_pTitleBar,SIGNAL(signalClose()),
            this,SLOT(slotClose()));
}

void QBaseWidget::initStyle()
{
    //加载样式表
    QFile file(":/image/css/style.css");
    file.open(QFile::ReadOnly);
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
    this->setStyleSheet(stylesheet);
    file.close();

    //设置编码方式
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    QTextCodec *codec = QTextCodec::codecForName("System");
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForTr(codec);
#endif
    //加载中文
    QTranslator *translator = new QTranslator(qApp);
    translator->load(":/image/translator/qt_zh_CN.qm");
    qApp->installTranslator(translator);
}

void QBaseWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_bIsPressed && (event->buttons() && Qt::LeftButton))
    {
        this->move(event->globalPos() - m_pressGlobal);
        event->accept();
    }
}

void QBaseWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_bIsPressed = true;
        m_pressGlobal = event->globalPos() - this->pos();
        event->accept();
    }
}

void QBaseWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_bIsPressed = false;
    }
}

void QBaseWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QLinearGradient linearGradient(0, 0, this->width(), this->height());
    linearGradient.setColorAt(0, QColor("#B3D3EF"));
    linearGradient.setColorAt(1, QColor("#83B0D7"));;
    painter.setBrush(QBrush(linearGradient));
    painter.drawRect(0, 0, this->width(), this->height());

    QBitmap objBitmap(size());
    //QPainter用于在位图上绘画
    QPainter p(&objBitmap);
    //填充位图矩形框(用白色填充)
    p.fillRect(rect(),Qt::white);
    p.setBrush(QColor(0,0,0));
    //在位图上画圆角矩形(用黑色填充)
    p.drawRoundedRect(0, 0, width() + 1, height() + 1, 10, 10);
    //使用setmask过滤即可
    setMask(objBitmap);

    //    QPainterPath path;
    //    path.setFillRule(Qt::WindingFill);
    //    path.addRect(0, 0, this->width()-10, this->height()-10);

    //    QPainter p(this);
    //    p.setRenderHint(QPainter::Antialiasing, true);
    //    p.fillPath(path, QBrush(Qt::white));

    //    QColor color(0, 0, 0, 50);
    //    for(int i = 0; i < 10; i++)
    //    {
    //        QPainterPath path;
    //        path.setFillRule(Qt::WindingFill);
    //        path.addRect(10 - i, 10 - i, this->width() - (10 - i)*2, this->height()-(10 - i)*2);
    //        color.setAlpha(150 - qSqrt(i)*50);
    //        p.setPen(color);
    //        p.drawPath(path);
    //    }
}

void QBaseWidget::slotShowMin()
{
    this->showMinimized();
}

void QBaseWidget::slotShowMax()
{
    this->showMaximized();
}

void QBaseWidget::slotClose()
{
    this->close();
}


void QBaseWidget::init()
{
}
