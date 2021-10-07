#ifndef QSTATUBAR_H
#define QSTATUBAR_H

#include <QWidget>
#include <QToolButton>

class QLabel;
class QPushButton;
class QPushButton;
class QHBoxLayout;
class QCursor;

class QStatubar : public QWidget
{
    Q_OBJECT
public:
   QStatubar(QWidget *parent = 0);
   ~QStatubar();

   void setStatueIcon(const QString &);     //设置状态栏图标
   void setAuthorName(const QString &);     //设置开发者名称
   void setCompanyName(const QString &);    //设置公司名称
    
private:
    //创建子部件
    void initWidget();
private:
    QLabel *m_pLabelIcon;
    QLabel *m_pLabelAuthor;
    QLabel *m_pLabelCompany;
    QHBoxLayout *m_pLayout;
    QCursor *mouseCursor;
    
};

#endif // QSTATUBAR_H
