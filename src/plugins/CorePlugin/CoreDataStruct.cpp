#include "CoreDataStruct.h"


ListItemData::ListItemData()
{
    reset();
}

ListItemData::~ListItemData()
{
    reset();
}

void ListItemData::operator=(const ListItemData &dsItem)
{
    m_bTitleInfo            = dsItem.m_bTitleInfo;
    m_strPatientName        = dsItem.m_strPatientName;
    m_strPatientBirthday    = dsItem.m_strPatientBirthday;
    m_strSeriesDataTime     = dsItem.m_strSeriesDataTime;
    m_strModaly             = dsItem.m_strModaly;
    m_nSeriesNumber         = dsItem.m_nSeriesNumber;
    m_strDescription        = dsItem.m_strDescription;
    m_baThumbnailBuffer     = dsItem.m_baThumbnailBuffer;
    m_szThumb               = dsItem.m_szThumb;
}

QDataStream &ListItemData::toStream(QDataStream &stream)
{
    stream << m_bTitleInfo
           << m_strPatientName
           << m_strPatientBirthday
           << m_strSeriesDataTime
           << m_strModaly
           << m_nSeriesNumber
           << m_strDescription
           << m_baThumbnailBuffer
           << m_szThumb;

    return stream;
}

QDataStream &ListItemData::fromStream(QDataStream &stream)
{
    stream >> m_bTitleInfo
           >> m_strPatientName
           >> m_strPatientBirthday
           >> m_strSeriesDataTime
           >> m_strModaly
           >> m_nSeriesNumber
           >> m_strDescription
           >> m_baThumbnailBuffer
           >> m_szThumb;

    return stream;
}

void ListItemData::reset()
{
    m_bTitleInfo            = true;
    m_strPatientName        = "";
    m_strPatientBirthday    = "";
    m_strSeriesDataTime     = "";
    m_strModaly             = "";
    m_nSeriesNumber         = -1;
    m_strDescription        = "";
    m_baThumbnailBuffer.clear();
    m_szThumb               = QSize(0, 0);
}

bool ListItemData::copyFrom(ListItemData *pSrc)
{
    if (nullptr == pSrc)
    {
        return false;
    }

    m_bTitleInfo            = pSrc->m_bTitleInfo;
    m_strPatientName        = pSrc->m_strPatientName;
    m_strPatientBirthday    = pSrc->m_strPatientBirthday;
    m_strSeriesDataTime     = pSrc->m_strSeriesDataTime;
    m_strModaly             = pSrc->m_strModaly;
    m_nSeriesNumber         = pSrc->m_nSeriesNumber;
    m_strDescription        = pSrc->m_strDescription;
    m_baThumbnailBuffer     = pSrc->m_baThumbnailBuffer;
    m_szThumb               = pSrc->m_szThumb;

    return true;
}
