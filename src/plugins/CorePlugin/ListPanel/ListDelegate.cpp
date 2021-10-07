#include "ListDelegate.h"
#include "../CoreInclude.h"

#include "../CoreDataStruct.h"
#include "../coreconstants.h"
#include "../CoreUtil.h"


QSize GetTextSize(const QFont* pFont, const QString& strText )
{
    QFontMetrics fm( *pFont );
    return fm.size( Qt::TextExpandTabs, strText );
}

using namespace Core;
#define MARGIN 8

class ListDelegatePrivate
{
public:
    ListDelegatePrivate(){
        m_nGroupHeight = Constants::LIST_PANEL_WIDTH /3;
        m_nThumbHeight = m_nGroupHeight*2 + 14;
    }

    int m_nGroupHeight;
    int m_nThumbHeight;
};

ListDelegate::ListDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
    , m_pImpl(new ListDelegatePrivate)
{

}

ListDelegate::~ListDelegate()
{
    if (nullptr != m_pImpl)
    {
        delete m_pImpl;
        m_pImpl = nullptr;
    }
}

void ListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return;
    }

    painter->save();

    QVariant variant = index.data(Qt::UserRole+1);
    ListItemData data = variant.value<ListItemData>();

    QRect rect;
    rect.setX(option.rect.x());
    rect.setY(option.rect.y());
    rect.setWidth( option.rect.width()- MARGIN);
    rect.setHeight(option.rect.height()- MARGIN);

    QColor coThumbBack = Constants::LIST_PANEL_CL_NORMAL;

    //绘制数据位置
    QFont font("Times", 11, QFont::Bold);
    painter->setPen(QPen(QColor(223,223,223)));
    painter->setFont(font);

    if(option.state.testFlag(QStyle::State_Selected))
    {
        coThumbBack = Constants::LIST_PANEL_CL_ACTIVE;
    }
    else if(option.state.testFlag(QStyle::State_MouseOver))
    {
        coThumbBack = Constants::LIST_PANEL_CL_HOVER;
    }

    // 绘制边框
    painter->drawRect(rect);

    if (data.m_bTitleInfo)
    {
        /*====================================== 病人信息============================*/
        QRect patientBorderRect = rect;
        patientBorderRect.setHeight(m_pImpl->m_nGroupHeight);
        painter->fillRect(patientBorderRect, QColor(95,65,65));

        QRect patientNameRect = QRect(QPoint(0, 0),GetTextSize(&font,data.m_strPatientName));
        patientNameRect.moveTo( (rect.width() - patientNameRect.width())/2, rect.top());
        // 病人姓名
        painter->drawText(patientNameRect,data.m_strPatientName);

        font.setPixelSize(10);
        painter->setFont(font);

        //病人出生日期
        QRect patientBirthdayRect = QRect(QPoint(0, 0),GetTextSize(&font,data.m_strPatientBirthday));
        patientBirthdayRect.moveTo( (rect.width() - patientBirthdayRect.width())/2, patientBorderRect.height()/2);
        painter->drawText(patientBirthdayRect,data.m_strPatientBirthday);

        /*====================================== 序列信息============================*/
        QRect seriesBorderRect = patientBorderRect;
        seriesBorderRect.moveTo(patientBorderRect.bottomLeft());
        painter->fillRect(seriesBorderRect, QColor(109,85,85));

        // 序列时间
        QRect seriesTimeRect = QRect(QPoint(0, 0),GetTextSize(&font,data.m_strSeriesDataTime));
        seriesTimeRect.moveTo( (rect.width() - seriesTimeRect.width())/2, seriesBorderRect.top());
        painter->drawText(seriesTimeRect,data.m_strSeriesDataTime);

        // 设备类型，序列数
        QString strText = QString(tr("%1: %2 series")).arg(data.m_strModaly).arg(data.m_nSeriesNumber);
        QRect modalyRect = QRect(QPoint(0, 0),GetTextSize(&font,strText));
        modalyRect.moveTo( (rect.width() - modalyRect.width())/2, seriesBorderRect.bottom() - seriesBorderRect.height() / 2);
        painter->drawText(modalyRect,strText);
    }
    else
    {
        /*====================================== 缩略图+序列描述============================*/
        font.setPixelSize(10);
        painter->setFont(font);

        QRect thumbnailImageRect = rect;
        thumbnailImageRect.setHeight(m_pImpl->m_nThumbHeight);

        QSize szScal = QSize(thumbnailImageRect.width() * 0.6, thumbnailImageRect.width()*0.6);

        QImage tmpImage = CoreUtil::imageFromBuffer(data.m_baThumbnailBuffer);
        QImage image = tmpImage.scaled(szScal, Qt::KeepAspectRatio);

        QRect targetRect;
        targetRect.setX((thumbnailImageRect.width() - image.width())/2);
        targetRect.setY((thumbnailImageRect.height() - image.height())/2 + thumbnailImageRect.top());
        targetRect.setSize(szScal);

        QRect imgSourceRect = QRect(0,0,image.width(),image.height());

        painter->fillRect(thumbnailImageRect, coThumbBack);
        painter->drawRect(targetRect);

        // 序列缩略图
        painter->drawImage(targetRect, image, imgSourceRect);

        // 序列描述信息
        QSize desSize = GetTextSize(&font,data.m_strDescription);
        QRect descriptionRect = QRect(QPoint(0, 0),desSize);
        descriptionRect.moveTo( (rect.width() - targetRect.width())/2, targetRect.bottom() + 4);

        painter->setPen(QPen(QColor(15,15,15)));
        painter->drawText(descriptionRect, data.m_strDescription);
    }

    painter->restore();
}

QSize ListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);

    QVariant variant = index.data(Qt::UserRole+1);
    ListItemData data = variant.value<ListItemData>();

    int nHeight = 2 * m_pImpl->m_nGroupHeight;
    if (!data.m_bTitleInfo)
    {
        nHeight = m_pImpl->m_nThumbHeight + MARGIN;
    }

    return QSize(Constants::LIST_PANEL_WIDTH, nHeight);
}

bool ListDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    Q_UNUSED(event);
    Q_UNUSED(model);
    Q_UNUSED(option);
    Q_UNUSED(index);

    if (event->type() == QEvent::DragEnter
            ||event->type() == QEvent::DragMove)
    {
        return QStyledItemDelegate::editorEvent(event,model, option, index);
    }

    return true;
}

bool ListDelegate::helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(!index.isValid())
    {
        return false;
    }

    if (event->type() == QEvent::ToolTip)
    {
        QVariant variant = index.data(Qt::UserRole+1);
        ListItemData data = variant.value<ListItemData>();
        if (data.m_bTitleInfo)
        {
            return false;
        }
        QString strTip = QString(tr("%1\r\n\r\n%2").arg(data.m_strSeriesDataTime)
                                 .arg("Hold down Ctrl key and click image\r\nto open series in new panel."));

        QToolTip::showText(QCursor::pos(), strTip, reinterpret_cast<QWidget*>(view)
                           , option.rect, Constants::LIST_PANEL_TOOLTIP_TIME);
    }
    return true;
}
