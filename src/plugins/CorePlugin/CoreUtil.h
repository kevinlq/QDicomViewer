#ifndef COREUTIL_H
#define COREUTIL_H

#include "CoreInclude.h"
#include "core_global.h"

class ListItemData;

namespace Core{

class CORE_EXPORT CoreUtil
{
public:
    static QByteArray imageToBuffer(const QImage* pImage,const char* format = nullptr);
    static QImage imageFromBuffer( const QByteArray&buffer,const char* format = nullptr);

    static bool isValidFile(const QUrl &url, bool bCheckFile, const QString &strFilter);
    static bool isValidDicomFile(const QString &strFilePath);
    static bool isValidDicomFile(const QFileInfo &fileInfo);
    static bool parseFileFromUrl(const QList<QUrl> &lsUrl, QVariantList &lsFile);

    static bool getFile(const QString& strFilePath, const QStringList &filters, QList<QFileInfo> &lsFileInfo);
    static bool getDicomFiles(const QVariantList &lsFiles, QList<QFileInfo> &lsFileInfo);

    static QByteArray listItemDataToBuffer(ListItemData*pData);
    static ListItemData* listItemDataFromBuffer(QByteArray &buffer);
};
}

#endif // COREUTIL_H
