#ifndef ACTIONCONTAINERPRIVATE_H
#define ACTIONCONTAINERPRIVATE_H


#include "ActionContainer.h"

namespace Core {
namespace Internal {

struct Group
{
    Group(Id id) : id(id) {}
    Id id;
    QList<QObject *> items; // Command * or ActionContainer *
};

class ActionContainerPrivate : public ActionContainer
{
    Q_OBJECT
public:
    ActionContainerPrivate(Id id);

    virtual Id id() const;
    virtual void clear();
    virtual void appendGroup(Id group);
    virtual void insertGroup(Id before, Id groupId);
    virtual void addMenu(ActionContainer *menu, Id groupId);

    virtual QMenu *menu() const;

    QList<Group> m_groups;

protected:
    bool canAddMenu(ActionContainer *menu) const;
    virtual bool canBeAddedToMenu() const = 0;

    virtual void insertMenu(QAction *before, QMenu *menu) = 0;

    void itemDestroyed();

    QList<Group>::const_iterator findGroup(Id groupId) const;

private:
    Id  m_id;
};

class ToolMenuActionContainer: public ActionContainerPrivate
{
    Q_OBJECT
public:
    ToolMenuActionContainer(Id id);
    ~ToolMenuActionContainer();

    virtual QMenu *menu() const;

    void insertAction(QAction *before, QAction *action);
    void insertMenu(QAction *before, QMenu *menu);

    void removeAction(QAction *action);
    void removeMenu(QMenu *menu);

protected:
    virtual bool canBeAddedToMenu() const;

private:
    QMenu*  m_pMenu;
};

class ToolMenuPanelContainer : public ActionContainerPrivate
{
    Q_OBJECT
public:
    ToolMenuPanelContainer(Id id);
    ~ToolMenuPanelContainer();

    void insertAction(QAction *before, QAction *action);
    void insertMenu(QAction *before, QMenu *menu);

    void removeAction(QAction *action);
    void removeMenu(QMenu *menu);
protected:
    virtual bool canBeAddedToMenu() const;

public:
    QList<ActionContainer*> m_pMenus;
};

// test
enum MenuItemType{MenuItemType_Null = -1,MenuItemType_Menu, MenuItemType_Action, MenuItemType_Size};
struct DS_ToolMenuItem
{
    Id  m_id;
    MenuItemType    m_eType;
};

struct DS_ToolMenu
{
    QVector<DS_ToolMenuItem*>   m_vtrpToolMenu;
};

struct DS_ToolMenuPanel
{
    Id  m_nToolPanel;
    QVector<DS_ToolMenu*>   m_vtrpPanelMenu;
};

} // namespace Internal
} // namespace Core
#endif // ACTIONCONTAINERPRIVATE_H
