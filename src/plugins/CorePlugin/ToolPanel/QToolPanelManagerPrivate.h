#ifndef QTOOLPANELMANAGERPRIVATE_H
#define QTOOLPANELMANAGERPRIVATE_H

#include "../id.h"

#include <QObject>

#include <QMap>
#include <QHash>
#include <QMultiHash>
#include <QTimer>

QT_BEGIN_NAMESPACE
class QLabel;
class QSettings;
QT_END_NAMESPACE

namespace Core {
class ActionContainer;

namespace Internal {

class QToolPanelManagerPrivate : public QObject
{
    Q_OBJECT
public:
    typedef QHash<Id, ActionContainer *> IdContainerMap;
    explicit QToolPanelManagerPrivate(QObject *parent = nullptr);

    void containerDestroyed();

signals:
public:
    IdContainerMap m_idContainerMap;

};

} // namespace Internal
} // namespace Core

#endif // QTOOLPANELMANAGERPRIVATE_H
