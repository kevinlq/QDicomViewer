#ifndef CONTENTWIDGETCONTAINER_H
#define CONTENTWIDGETCONTAINER_H

#include <QWidget>

class QListPanel;
class ViewPanel;
class QStatusPanel;

class ContentWidgetContainer : public QWidget
{
    Q_OBJECT
public:
    explicit ContentWidgetContainer(QWidget *parent = nullptr);
    ~ContentWidgetContainer();

    bool init();

protected:
    QListPanel*     m_pListPanel;
    ViewPanel*      m_pViewPanel;
    QStatusPanel*   m_pStatusPanel;
};

#endif // CONTENTWIDGETCONTAINER_H
