#include "AppMainWidget.h"
#include "theme/theme_p.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include <QEvent>
#include <QCoreApplication>

#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

using namespace Utils;

Q_LOGGING_CATEGORY(pluginLog, "QDicomViewer.utils")

/* The notification signal is delayed by using a custom event
 * as otherwise device removal is not detected properly
 * (devices are still present in the registry. */

class DeviceNotifyEvent : public QEvent {
public:
    explicit DeviceNotifyEvent(int id) : QEvent(static_cast<QEvent::Type>(id)) {}
};

class AppMainWidgetPrivate
{
public:
    AppMainWidgetPrivate(AppMainWidget* q)
        :q_ptr(q)
    {
    }

    AppMainWidget* q_ptr;
};

AppMainWidget::AppMainWidget(QWidget *parent)
    : QWidget(parent)
    , m_deviceEventId(QEvent::registerEventType(QEvent::User + 2))
    , m_pMainWidgetImpl(new(std::nothrow) AppMainWidgetPrivate(this))
{
    //setAttribute(Qt::WA_TranslucentBackground,true);
    setObjectName("widgetMain");
}

AppMainWidget::~AppMainWidget()
{
    if (nullptr != m_pMainWidgetImpl)
    {
        delete m_pMainWidgetImpl;
        m_pMainWidgetImpl = nullptr;
    }
}

void AppMainWidget::raiseWindow()
{
    setWindowState(windowState() & ~Qt::WindowMinimized);

    raise();

    activateWindow();
}

#ifdef Q_OS_WIN
bool AppMainWidget::event(QEvent *event)
{
    const QEvent::Type type = event->type();
    if (type == m_deviceEventId) {
        event->accept();
        emit deviceChange();
        return true;
    }
    if (type == QEvent::ThemeChange)
        setThemeApplicationPalette();
    return QWidget::event(event);
}

bool AppMainWidget::winEvent(MSG *msg, long *result)
{
    if (msg->message == WM_DEVICECHANGE) {
        if (msg->wParam & 0x7 /* DBT_DEVNODES_CHANGED */) {
            *result = TRUE;
            QCoreApplication::postEvent(this, new DeviceNotifyEvent(m_deviceEventId));
        }
    }
    return false;
}
#endif
