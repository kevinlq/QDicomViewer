#ifndef QBASEWIDGET_H
#define QBASEWIDGET_H

#include <QFrame>

class QVBoxLayout;
class QTitlebar;
class QStatubar;

class QBaseWidget : public QFrame
{
    Q_OBJECT
public:

    static void init();

    //为标题栏和状态栏提供修改接口
    QBaseWidget(bool bShowTitleBar,QWidget *pContentWidget,
                bool bShowStatuBar,QWidget *parent = 0);

    //为标题栏提供修改接口
    void setWidgetBtnShow(bool bMinBtnShow,bool bMenuBtnShw,
                          bool bMaxBtnShow,bool bCloseBtnShow);

    //设置窗口标题栏文本
    void setWidgetTitleText(const QString &str);
    //设置窗口图标
    void setWidgetIcon(const QString &path);

    //状态栏一些设置
    void setStatueIcon(const QString &);
    void setAuthorName(const QString &);
    void setCompanyName(const QString &);

    //get widgete close statue
    bool getClosing();
    bool getBoolMaxWin();

private:
    void initForm();
    void initConnect();
    void initStyle();

protected:
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void paintEvent(QPaintEvent *);

private Q_SLOTS:
    void slotShowMin();
    void slotShowMax();
    void slotClose();

private:
    QPoint m_pressGlobal;
    bool m_bIsPressed;

    bool m_bLeftBtnPress;                               //鼠标左键是否被按下
    QPoint m_ptPressGlobal;                             //主窗口需要移动的相对位移
//    enum_Direction m_eDirection;                        //伸缩方向
    bool m_bMaxWin;                                     //是否已经最大化
    QRect m_rectRestoreWindow;                          //窗口恢复的大小

    bool m_bIsClose;

protected:
    QVBoxLayout *m_pMainLayout;     //主布局
    QTitlebar *m_pTitleBar;         //标题栏
    QWidget *m_pContentWidget;      //内容区域，中央部件
    QStatubar *m_pStatuBar;         //状态栏
};

#endif // QBASEWIDGET_H
