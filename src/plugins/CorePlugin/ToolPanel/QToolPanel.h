#ifndef QTOOLPANEL_H
#define QTOOLPANEL_H

#include <QWidget>

#include <CorePlugin/id.h>

QT_BEGIN_NAMESPACE
class QMenu;
class QAction;
class QHBoxLayout;
QT_END_NAMESPACE

namespace Core {

class QToolPanel : public QWidget
{
    Q_OBJECT
public:
    explicit QToolPanel(QWidget *parent = nullptr);
    ~QToolPanel();

    bool addMenu(const Id &id);

protected:
    virtual void paintEvent(QPaintEvent *event);

public Q_SLOTS:
    void updateToolPanel();

private:
    QHBoxLayout*    m_pHLayout;
};
}

#endif // QTOOLPANEL_H
