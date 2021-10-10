#include "CoreUtil.h"
#include "CoreInclude.h"

#include "CoreDataStruct.h"
#include "coreconstants.h"

using namespace Core;

QByteArray CoreUtil::imageToBuffer(const QImage *pImage, const char* format)
{
    QByteArray baBuffer;
    QBuffer buffer(&baBuffer);
    buffer.open(QIODevice::WriteOnly);

    const char* pFormat = format == nullptr ? Constants::THUMBNAIL_FORMAT : format;

    pImage->save(&buffer, pFormat);

    return baBuffer;
}

QImage CoreUtil::imageFromBuffer(const QByteArray &buffer, const char* format)
{
    const char* pFormat = format == nullptr ? Constants::THUMBNAIL_FORMAT : format;

    QImage pImage;
    pImage = QImage::fromData(buffer, pFormat);
    return pImage;
}

bool CoreUtil::isValidFile(const QUrl &url, bool bCheckFile,const QString &strFilter)
{
    bool bValidFile = false;
    QFileInfo info(url.toLocalFile());

    if (info.isFile())
    {
        QString strFileName = info.fileName();
        QStringList lsFilter = strFilter.split("|");

        if (lsFilter.isEmpty())
        {
            lsFilter << ".dcm";
        }

        for (int i = 0; i < lsFilter.size(); i++)
        {
            if (strFileName.contains(lsFilter[i]))
            {
                bValidFile = true;
                break;
            }
        }
    }
    else
    {
        bValidFile = bCheckFile;
    }

    return bValidFile;
}

bool CoreUtil::isValidDicomFile(const QString &strFilePath)
{
    QFileInfo info(strFilePath);

    QStringList lsFilter = QString(Constants::DROP_EXTEND_FILTER).split("|");

    return isValidDicomFile(info, lsFilter);
}

bool CoreUtil::isValidDicomFile(const QFileInfo &fileInfo,const QStringList &filters)
{
    if (fileInfo.isDir())
    {
        return false;
    }

    QString strFileName = fileInfo.fileName();
    QStringList lsFilter = filters;

    if (lsFilter.isEmpty())
    {
        lsFilter << ".dcm";
    }

    for (int i = 0; i < lsFilter.size(); i++)
    {
        if (strFileName.contains(lsFilter[i]))
        {
            return true;
        }
    }

    return false;
}

bool CoreUtil::parseFileFromUrl(const QList<QUrl> &lsUrl, QVariantList &lsFile)
{
    lsFile.clear();

    if (lsUrl.isEmpty())
    {
        return false;
    }

    for (int i = 0; i < lsUrl.size(); i++)
    {
        lsFile.append(lsUrl[i].toLocalFile());
    }

    return true;
}

bool CoreUtil::getFile(const QString &strFilePath, const QStringList &filters, QList<QFileInfo> &lsFileInfo)
{
    QDir dir(strFilePath);

    if(!dir.exists())
    {
        lsFileInfo.push_back(QFileInfo(strFilePath));
        return true;
    }

    //取到所有的文件和文件名，去掉.和..文件夹
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::DirsFirst);

    //将其转化为一个list
    QFileInfoList list = dir.entryInfoList();
    if(list.size()<1)
        return false;

    int i = 0;
    //采用递归算法
    do {
        QFileInfo fileInfo = list.at(i);
        bool bisDir = fileInfo.isDir();
        if(bisDir)
        {
            getFile(fileInfo.filePath(), filters, lsFileInfo);
        }
        else
        {
            if (isValidDicomFile(fileInfo, filters))
            {
               lsFileInfo.push_back(fileInfo);
            }
        }

        ++i;
    }
    while(i<list.size());

    return false;
}

bool CoreUtil::getDicomFiles(const QVariantList &lsFiles,QList<QFileInfo> &lsFileInfos)
{
    QStringList filters = QString(Constants::DICOM_FILTER).split("|");

    for (int i = 0; i < lsFiles.size(); i++)
    {
        QFileInfo fileInfo(lsFiles[i].toString());

        if (fileInfo.isFile())
        {
            if (isValidDicomFile(fileInfo, filters))
            {
                lsFileInfos.push_back(fileInfo);
            }
        }
        else
        {
            getFile(fileInfo.filePath(), filters, lsFileInfos);
        }
    }

    return true;
}

QStringList CoreUtil::getFiles(bool bFolder, const QStringList &filters)
{
    QFileDialog fileDialog;
    fileDialog.setNameFilters(filters);
    fileDialog.setViewMode(QFileDialog::Detail);
    fileDialog.setFileMode(bFolder ? QFileDialog::Directory: QFileDialog::ExistingFiles);
    fileDialog.setWindowTitle(QObject::tr("select file or folder"));

    QStringList fileNames;
    if (fileDialog.exec())
        fileNames = fileDialog.selectedFiles();

    return fileNames;
}

QByteArray CoreUtil::listItemDataToBuffer(ListItemData *pData)
{
    QByteArray buffer;
    QDataStream stream( &buffer, QIODevice::WriteOnly );
    stream.setVersion(Constants::G_STREAM_VERSION);

    pData->toStream(stream);

    return buffer;
}

ListItemData *CoreUtil::listItemDataFromBuffer(QByteArray &buffer)
{
    ListItemData* pData = new(std::nothrow) ListItemData;

    QDataStream stream( &buffer, QIODevice::ReadOnly );
    stream.setVersion(Constants::G_STREAM_VERSION);

    pData->fromStream(stream);

    return pData;
}
