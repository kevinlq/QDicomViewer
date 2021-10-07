#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QWidget>
#include "../utils_global.h"

QT_BEGIN_NAMESPACE
class QTitlebar;
class QToolPanel;
class QStatubar;
class QVBoxLayout;
QT_END_NAMESPACE

namespace Utils {

class QRADIANT_UTILS_EXPORT BaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BaseWidget(bool bTitle,bool bStatus,QWidget *parent = nullptr);
    virtual~BaseWidget();

    void setAppTitleIcon(const QString &strPath);
    void setAppTitleInfo(const QString& strInfo);

    void setContentWidget(QWidget* p);

protected:
    QTitlebar*      m_pTitleBar;
    QStatubar*      m_pStatusBar;
    QWidget*        m_pContentWidget;      //内容区域，中央部件
    QVBoxLayout*    m_pMainLayout;          //主布局
};

}

#endif // BASEWIDGET_H
