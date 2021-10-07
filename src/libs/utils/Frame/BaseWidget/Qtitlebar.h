#ifndef QTITLEBAR_H
#define QTITLEBAR_H

#include <QWidget>

#include <QPoint>

#define VALUE_DIS   5

class QMouseEvent;
class QPaintEvent;
class QLabel;
class QAbstractButton;
class QPushButton;
class QToolButton;
class QHBoxLayout;
class QCursor;
class QAction;
class QMenu;
class QVBoxLayout;

class QTitlebar : public QWidget
{
    Q_OBJECT
public:
    explicit QTitlebar(QWidget *parent = 0);
    ~QTitlebar();

    //设置标题栏标题
    void setTitleText(const QString &);
    //设置窗口图标
    void setTitleIcon(const QString &);
    
private:
    void initForm();
    //创建子部件
    void initWidget();
    void initConnect();

Q_SIGNALS:
    void signalMin();
    void signalMax();
    void signalClose();

public:
    QLabel *m_pLabelIcon;       //程序图标
    QLabel *m_pLabelTitle;      //程序标题
    QToolButton *m_pBtnMenu;    //皮肤
    QPushButton *m_pBtnMin;     //最小化
    QPushButton *m_pBtnMax;     //最大化
    QPushButton *m_pBtnClose;   //关闭

    QHBoxLayout *m_pLayout;     //控件水平布局
    QVBoxLayout *m_pMainLayout; //主布局
    
};

#endif // QTITLEBAR_H
