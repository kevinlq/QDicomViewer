#ifndef VIEWPANEL_H
#define VIEWPANEL_H

#include <QWidget>

struct DS_LayoutInfo
{
    DS_LayoutInfo(){}
    DS_LayoutInfo(int nID, QRect rcView)
        : m_nViewID(nID), m_rcView(rcView)
    {}

    int m_nViewID = -1;
    QRect   m_rcView;
};

class DImageView;
typedef QVector<DImageView*> VTRP_IMAGEVIEW;
typedef QVector<DS_LayoutInfo> VTR_LAYOUT;

class ViewPanel : public QWidget
{
    Q_OBJECT
public:
    explicit ViewPanel(QWidget *parent = 0);
    ~ViewPanel();

    void updateViewLayout();
    void updateLayout(int nRow, int nCol);

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

Q_SIGNALS:
    void signalAddDicom(const QVariantList &lsFiles);

public Q_SLOTS:
    void onSignalEvent(int nEventID, const QVariant &param);

private:
    DImageView* getImageView(int nViewID);
    bool clearAllImageView();
    void hideAllImagView();
    void setAllImageViewActived(bool bActived);
    bool setImageViewActived(int nViewID,bool bActived);

    void updateLayoutInfo();
    void updateViewLayoutPos();

private:
    VTRP_IMAGEVIEW  m_vtrpImageView;
    VTR_LAYOUT      m_vtrLayoutInfo;
    int             m_nActivedView;
    bool            m_bSingleView;
};

#endif // VIEWPANEL_H
