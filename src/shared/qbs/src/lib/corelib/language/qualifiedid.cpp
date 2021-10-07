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

#include "qualifiedid.h"

#include <algorithm>

namespace qbs {
namespace Internal {

QualifiedId::QualifiedId()
{
}

QualifiedId::QualifiedId(const QString &singlePartName)
    : QStringList(singlePartName)
{
}

QualifiedId::QualifiedId(const QStringList &nameParts)
    : QStringList(nameParts)
{
}

QualifiedId QualifiedId::fromString(const QString &str)
{
    return QualifiedId(str.split(QLatin1Char('.')));
}

QString QualifiedId::toString() const
{
    return join(QLatin1Char('.'));
}

bool operator<(const QualifiedId &a, const QualifiedId &b)
{
    const int c = std::min(a.count(), b.count());
    for (int i = 0; i < c; ++i) {
        int n = a.at(i).compare(b.at(i));
        if (n < 0)
            return true;
        if (n > 0)
            return false;
    }
    return a.count() < b.count();
}

} // namespace Internal
} // namespace qbs
