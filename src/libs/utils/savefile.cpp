/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of QDcmViewer.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#include "savefile.h"
#include "qtcassert.h"
#include "fileutils.h"
#ifdef Q_OS_WIN
#  include <windows.h>
#  include <io.h>
#else
#  include <unistd.h>
#  include <sys/stat.h>
#endif

namespace Utils {

QFile::Permissions SaveFile::m_umask = 0;

SaveFile::SaveFile(const QString &filename) :
    m_finalFileName(filename), m_finalized(true), m_backup(false)
{
}

SaveFile::~SaveFile()
{
    QTC_ASSERT(m_finalized, rollback());
}

bool SaveFile::open(OpenMode flags)
{
    QTC_ASSERT(!m_finalFileName.isEmpty() && fileName().isEmpty(), return false);

    QFile ofi(m_finalFileName);
    // Check whether the existing file is writable
    if (ofi.exists() && !ofi.open(QIODevice::ReadWrite)) {
        setErrorString(ofi.errorString());
        return false;
    }

    setAutoRemove(false);
    setFileTemplate(m_finalFileName);
    if (!QTemporaryFile::open(flags))
        return false;

    m_finalized = false; // needs clean up in the end
    if (ofi.exists()) {
        setPermissions(ofi.permissions()); // Ignore errors
    } else {
        Permissions permAll = QFile::ReadOwner
                | QFile::ReadGroup
                | QFile::ReadOther
                | QFile::WriteOwner
                | QFile::WriteGroup
                | QFile::WriteOther;

        // set permissions with respect to the current umask
        setPermissions(permAll & ~m_umask);
    }

    return true;
}

void SaveFile::rollback()
{
    remove();
    m_finalized = true;
}

bool SaveFile::commit()
{
    QTC_ASSERT(!m_finalized, return false);
    m_finalized = true;

    if (!flush()) {
        remove();
        return false;
    }
#ifdef Q_OS_WIN
    FlushFileBuffers(reinterpret_cast<HANDLE>(_get_osfhandle(handle())));
#elif _POSIX_SYNCHRONIZED_IO > 0
    fdatasync(handle());
#else
    fsync(handle());
#endif
    close();
    if (error() != NoError) {
        remove();
        return false;
    }

    QString finalFileName
            = FileUtils::resolveSymlinks(FileName::fromString(m_finalFileName)).toString();
    QString bakname = finalFileName + QLatin1Char('~');

    if (QFile::exists(finalFileName)) {
        QFile::remove(bakname); // Kill old backup
        // Try to back up current file
        if (!QFile::rename(finalFileName, bakname)) {
            remove();
            setErrorString(tr("File might be locked."));
            return false;
        }
    }
    if (!rename(finalFileName)) { // Replace current file
        QFile::rename(bakname, finalFileName); // Rollback to current file
        remove();
        return false;
    }
    if (!m_backup)
        QFile::remove(bakname);

    return true;
}

void SaveFile::initializeUmask()
{
#ifdef Q_OS_WIN
    m_umask = QFile::WriteGroup | QFile::WriteOther;
#else
    // Get the current process' file creation mask (umask)
    // umask() is not thread safe so this has to be done by single threaded
    // application initialization
    mode_t mask = umask(0); // get current umask
    umask(mask); // set it back

    m_umask = ((mask & S_IRUSR) ? QFile::ReadOwner  : QFlags<QFile::Permission>(0))
            | ((mask & S_IWUSR) ? QFile::WriteOwner : QFlags<QFile::Permission>(0))
            | ((mask & S_IXUSR) ? QFile::ExeOwner   : QFlags<QFile::Permission>(0))
            | ((mask & S_IRGRP) ? QFile::ReadGroup  : QFlags<QFile::Permission>(0))
            | ((mask & S_IWGRP) ? QFile::WriteGroup : QFlags<QFile::Permission>(0))
            | ((mask & S_IXGRP) ? QFile::ExeGroup   : QFlags<QFile::Permission>(0))
            | ((mask & S_IROTH) ? QFile::ReadOther  : QFlags<QFile::Permission>(0))
            | ((mask & S_IWOTH) ? QFile::WriteOther : QFlags<QFile::Permission>(0))
            | ((mask & S_IXOTH) ? QFile::ExeOther   : QFlags<QFile::Permission>(0));
#endif
}

} // namespace Utils
