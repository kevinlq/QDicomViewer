/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qbs.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QBS_FILETIME_H
#define QBS_FILETIME_H

#include "persistence.h"

#include <QtCore/qdatastream.h>
#include <QtCore/qdebug.h>

#if defined(Q_OS_UNIX)
#include <time.h>
#define BUILD_HOST_HAS_CLOCK_GETTIME (_POSIX_C_SOURCE >= 199309L)
#ifdef __APPLE__
#if __MAC_OS_X_VERSION_MIN_REQUIRED >= 101200
#define HAS_CLOCK_GETTIME BUILD_HOST_HAS_CLOCK_GETTIME
#endif // __MAC_OS_X_VERSION_MIN_REQUIRED
#else // __APPLE__
#define HAS_CLOCK_GETTIME BUILD_HOST_HAS_CLOCK_GETTIME
#endif // __APPLE__
#endif // Q_OS_UNIX

namespace qbs {
namespace Internal {

class QBS_AUTOTEST_EXPORT FileTime
{
public:
#if defined(Q_OS_UNIX)
#if HAS_CLOCK_GETTIME
    typedef timespec InternalType;
#else
    typedef time_t InternalType;
#endif // HAS_CLOCK_GETTIME
#elif defined(Q_OS_WIN)
    typedef quint64 InternalType;
#else
#   error unknown platform
#endif

    FileTime();
    FileTime(const InternalType &ft);

    void store(PersistentPool &pool) const;
    void load(PersistentPool &pool);

    bool operator<(const FileTime &rhs) const { return compare(rhs) < 0; }
    bool operator>(const FileTime &rhs) const { return compare(rhs) > 0; }
    bool operator<=(const FileTime &rhs) const { return !operator>(rhs); }
    bool operator>=(const FileTime &rhs) const { return !operator<(rhs); }
    bool operator==(const FileTime &rhs) const { return compare(rhs) == 0; }
    bool operator!= (const FileTime &rhs) const { return !operator==(rhs); }
    int compare(const FileTime &other) const;

    void clear();
    bool isValid() const;
    QString toString() const;

    static FileTime currentTime();
    static FileTime oldestTime();

    double asDouble() const;

private:
    InternalType m_fileTime;
};

} // namespace Internal
} // namespace qbs

QT_BEGIN_NAMESPACE

inline QDebug operator<<(QDebug dbg, const qbs::Internal::FileTime &t)
{
    dbg.nospace() << t.toString();
    return dbg.space();
}

QT_END_NAMESPACE

#endif // QBS_FILETIME_H
