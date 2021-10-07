#include "QToolPanelManagerPrivate.h"

#include "ActionContainerPrivate.h"

namespace Core {

namespace Internal {
QToolPanelManagerPrivate::QToolPanelManagerPrivate(QObject *parent)
    : QObject(parent)
{

}

void QToolPanelManagerPrivate::containerDestroyed()
{
    ActionContainerPrivate *container = static_cast<ActionContainerPrivate *>(sender());
    m_idContainerMap.remove(m_idContainerMap.key(container));
}

} // namespace Internal
} // namespace Core

