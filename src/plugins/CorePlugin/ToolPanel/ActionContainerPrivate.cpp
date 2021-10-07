#include "ActionContainerPrivate.h"

#include "../CoreInclude.h"

#include "utils/qtcassert.h"
#include "../coreconstants.h"


using namespace Core;
using namespace Core::Internal;

ActionContainerPrivate::ActionContainerPrivate(Id id)
    : m_id(id)
{
}

Id ActionContainerPrivate::id() const
{
    return m_id;
}

void ActionContainerPrivate::clear()
{
}

void ActionContainerPrivate::appendGroup(Id group)
{
    m_groups.append(Group(group));
}

void ActionContainerPrivate::insertGroup(Id before, Id groupId)
{
    QList<Group>::iterator it = m_groups.begin();
    while (it != m_groups.end())
    {
        if (it->id == before)
        {
            m_groups.insert(it, Group(groupId));
            break;
        }
        ++it;
    }
}

void ActionContainerPrivate::addMenu(ActionContainer *menu, Id groupId)
{
    ActionContainerPrivate *containerPrivate = static_cast<ActionContainerPrivate *>(menu);
    if (!containerPrivate->canBeAddedToMenu())
    {
        return;
    }

    ToolMenuActionContainer* pContainer = static_cast<ToolMenuActionContainer*>(containerPrivate);

    const Id actualGroupId = groupId.isValid() ? groupId : Id(Constants::G_DEFAULT_TWO);
    QList<Group>::const_iterator groupIt = findGroup(actualGroupId);

    QTC_ASSERT(groupIt != m_groups.constEnd(), return);

    m_groups[groupIt - m_groups.constBegin()].items.append(menu);
    QAction *beforeAction = pContainer->menu()->menuAction();

    connect(menu, &QObject::destroyed, this, &ActionContainerPrivate::itemDestroyed);
    insertMenu(beforeAction, pContainer->menu());
}


QMenu *ActionContainerPrivate::menu() const
{
    return nullptr;
}

QList<Group>::const_iterator ActionContainerPrivate::findGroup(Id groupId) const
{
    QList<Group>::const_iterator it = m_groups.constBegin();
    while (it != m_groups.constEnd()) {
        if (it->id == groupId)
            break;
        ++it;
    }
    return it;
}

void ActionContainerPrivate::itemDestroyed()
{
    QObject *obj = sender();
    QMutableListIterator<Group> it(m_groups);
    while (it.hasNext())
    {
        Group &group = it.next();
        if (group.items.removeAll(obj) > 0)
            break;
    }
}

ToolMenuActionContainer::ToolMenuActionContainer(Id id)
    : ActionContainerPrivate(id)
    , m_pMenu(new QMenu)
{
    m_pMenu->setObjectName(id.toString());
}

ToolMenuActionContainer::~ToolMenuActionContainer()
{
    if (nullptr != m_pMenu)
    {
        delete m_pMenu;
        m_pMenu = nullptr;
    }
}

void ToolMenuActionContainer::insertAction(QAction *before, QAction *action)
{
    m_pMenu->insertAction(before, action);
}

void ToolMenuActionContainer::insertMenu(QAction *before, QMenu *menu)
{
    m_pMenu->insertMenu(before, menu);
}

void ToolMenuActionContainer::removeAction(QAction *action)
{
    m_pMenu->removeAction(action);
}

void ToolMenuActionContainer::removeMenu(QMenu *menu)
{
    m_pMenu->removeAction(menu->menuAction());
}

bool ToolMenuActionContainer::canBeAddedToMenu() const
{
    return true;
}

QMenu *ToolMenuActionContainer::menu() const
{
    return m_pMenu;
}

ToolMenuPanelContainer::ToolMenuPanelContainer(Id id)
    : ActionContainerPrivate(id)
{
}

ToolMenuPanelContainer::~ToolMenuPanelContainer()
{
}

void ToolMenuPanelContainer::insertAction(QAction *before, QAction *action)
{
}

void ToolMenuPanelContainer::insertMenu(QAction *before, QMenu *menu)
{
}

void ToolMenuPanelContainer::removeAction(QAction *action)
{
}

void ToolMenuPanelContainer::removeMenu(QMenu *menu)
{
}

bool ToolMenuPanelContainer::canBeAddedToMenu() const
{
    return false;
}

