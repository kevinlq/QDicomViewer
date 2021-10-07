#ifndef APPMAINWIDGET_H
#define APPMAINWIDGET_H

#include "Frame/BaseWidget.h"

class AppMainWidgetPrivate;

namespace Utils {

class QRADIANT_UTILS_EXPORT AppMainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AppMainWidget(QWidget *parent = nullptr);
    virtual ~AppMainWidget();

public slots:
    void raiseWindow();

signals:
    void deviceChange();

#ifdef Q_OS_WIN
protected:
    virtual bool winEvent(MSG *message, long *result);
    virtual bool event(QEvent *event);
#endif

private:
    const int m_deviceEventId;
    AppMainWidgetPrivate* m_pMainWidgetImpl;
};
}

#endif // APPMAINWIDGET_H
