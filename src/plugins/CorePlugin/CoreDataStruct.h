#ifndef COREDATASTRUCT_H
#define COREDATASTRUCT_H

#include "core_global.h"
QT_BEGIN_NAMESPACE
#include <QString>
#include <QImage>
#include <QVariant>
#include <QSize>
QT_END_NAMESPACE

// 列表数据结构
struct CORE_EXPORT ListItemData
{
    ListItemData();
    ~ListItemData();

    void operator=(const ListItemData &dsItem);
    QDataStream& toStream(QDataStream& stream);
    QDataStream& fromStream(QDataStream& stream);

    void reset();
    bool copyFrom(ListItemData* pSrc);

    bool        m_bTitleInfo;                       // 是否只显示标题信息不绘制缩略图
    QString     m_strPatientName;                   // 病人姓名
    QString     m_strPatientBirthday;               // 病人出身日期
    QString     m_strSeriesDataTime;                // 序列时间
    QString     m_strModaly;                        // 设备类型
    quint32     m_nSeriesNumber;                    // 序列号
    QString     m_strDescription;                   // 序列描述信息
    QByteArray  m_baThumbnailBuffer;                // 缩略图
    QSize       m_szThumb;                          // 缩略图大小(真正显示)
};
Q_DECLARE_METATYPE(ListItemData);

enum EventCode{
    EventActiveView = 0,
    EventDoubleClickView,
    EventCodeSize
};

#endif // COREDATASTRUCT_H
