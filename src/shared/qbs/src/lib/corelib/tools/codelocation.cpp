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
#include "codelocation.h"

#include <tools/fileinfo.h>
#include <tools/persistence.h>
#include <tools/qbsassert.h>

#include <QtCore/qdatastream.h>
#include <QtCore/qdir.h>
#include <QtCore/qregexp.h>
#include <QtCore/qshareddata.h>
#include <QtCore/qstring.h>

namespace qbs {

class CodeLocation::CodeLocationPrivate : public QSharedData
{
public:
    QString filePath;
    int line;
    int column;
};

CodeLocation::CodeLocation()
{
}

CodeLocation::CodeLocation(const QString &aFilePath, int aLine, int aColumn, bool checkPath)
    : d(new CodeLocationPrivate)
{
    QBS_ASSERT(!checkPath || Internal::FileInfo::isAbsolute(aFilePath), qDebug() << aFilePath);
    d->filePath = aFilePath;
    d->line = aLine;
    d->column = aColumn;
}

CodeLocation::CodeLocation(const CodeLocation &other) : d(other.d)
{
}

CodeLocation &CodeLocation::operator=(const CodeLocation &other)
{
    d = other.d;
    return *this;
}

CodeLocation::~CodeLocation()
{
}

QString CodeLocation::filePath() const
{
    return d ? d->filePath : QString();
}

int CodeLocation::line() const
{
    return d ? d->line : -1;
}

int CodeLocation::column() const
{
    return d ? d->column : -1;
}

bool CodeLocation::isValid() const
{
    return !filePath().isEmpty();
}

QString CodeLocation::toString() const
{
    QString str;
    if (isValid()) {
        str = QDir::toNativeSeparators(filePath());
        QString lineAndColumn;
        if (line() > 0 && !str.contains(QRegExp(QLatin1String(":[0-9]+$"))))
            lineAndColumn += QLatin1Char(':') + QString::number(line());
        if (column() > 0 && !str.contains(QRegExp(QLatin1String(":[0-9]+:[0-9]+$"))))
            lineAndColumn += QLatin1Char(':') + QString::number(column());
        str += lineAndColumn;
    }
    return str;
}

void CodeLocation::load(Internal::PersistentPool &pool)
{
    const bool isValid = pool.load<bool>();
    if (!isValid)
        return;
    d = new CodeLocationPrivate;
    pool.load(d->filePath);
    pool.load(d->line);
    pool.load(d->column);
}

void CodeLocation::store(Internal::PersistentPool &pool) const
{
    if (d) {
        pool.store(true);
        pool.store(d->filePath);
        pool.store(d->line);
        pool.store(d->column);
    } else {
        pool.store(false);
    }
}

bool operator==(const CodeLocation &cl1, const CodeLocation &cl2)
{
    if (cl1.d == cl2.d)
        return true;
    return cl1.filePath() == cl2.filePath() && cl1.line() == cl2.line()
            && cl1.column() == cl2.column();
}

bool operator!=(const CodeLocation &cl1, const CodeLocation &cl2)
{
    return !(cl1 == cl2);
}

QDebug operator<<(QDebug debug, const CodeLocation &location)
{
    return debug << location.toString();
}

bool operator<(const CodeLocation &cl1, const CodeLocation &cl2)
{
    return cl1.toString() < cl2.toString();
}

} // namespace qbs
