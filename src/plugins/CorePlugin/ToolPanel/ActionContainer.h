#ifndef ACTIONCONTAINER_H
#define ACTIONCONTAINER_H

#include <CorePlugin/core_global.h>
#include <CorePlugin/id.h>

QT_BEGIN_NAMESPACE
class QMenu;
class QAction;
QT_END_NAMESPACE

namespace Core {
class CORE_EXPORT ActionContainer : public QObject
{
    Q_OBJECT
public:
    explicit ActionContainer(QObject *parent = 0);
    virtual~ActionContainer();

    virtual Id id() const = 0;
    virtual void clear() = 0;
    virtual QMenu *menu() const = 0;
    virtual void appendGroup(Id group) = 0;
    virtual void insertGroup(Id before, Id group) = 0;
    virtual void addMenu(ActionContainer *menu, Id group) = 0;
};
}

#endif // ACTIONCONTAINER_H
