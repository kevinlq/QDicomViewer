#ifndef QSTATUSPANEL_H
#define QSTATUSPANEL_H

#include <QWidget>

class QLabel;
class QProgressBar;

class QStatusPanel : public QWidget
{
    Q_OBJECT
public:
    explicit QStatusPanel(QWidget *parent = 0);

protected:
    virtual void paintEvent(QPaintEvent *event);

public Q_SLOTS:
    void updateProgressValue(int nValue);

public:
    void setProgressRange(int nMin, int nMax);

private:
    QLabel*         m_pLabelInfo;
    QProgressBar*   m_pProgressBar;
};

#endif // QSTATUSPANEL_H
