#pragma once

#include <QWidget>

class DImageView : public QWidget
{
    Q_OBJECT
public:
    explicit DImageView(QWidget *parent = nullptr);
    ~DImageView();

    int getViewID() const;
    void setViewID(int nID);
    void setActived(bool bActive);

protected:
    virtual void paintEvent(QPaintEvent *event);

    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dragMoveEvent(QDragMoveEvent *event);
    virtual void dropEvent(QDropEvent *event);

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual void enterEvent(QEvent *event);

Q_SIGNALS:
    void signalEvent(int nEventID, const QVariant &param);

protected:
    int     m_nViewID;
    bool    m_bActived;
    bool    m_bHovered;
};

