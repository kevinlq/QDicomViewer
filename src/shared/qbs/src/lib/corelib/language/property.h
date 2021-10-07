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
#ifndef QBS_PROPERTY_H
#define QBS_PROPERTY_H

#include <tools/set.h>

#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>

namespace qbs {
namespace Internal {
class PersistentPool;

class Property
{
public:
    enum Kind
    {
        PropertyInModule,
        PropertyInProduct,
        PropertyInProject,
        PropertyInParameters
    };

    Property()
        : kind(PropertyInModule)
    {
    }

    Property(const QString &m, const QString &p, const QVariant &v, Kind k = PropertyInModule)
        : moduleName(m), propertyName(p), value(v), kind(k)
    {
    }

    void store(PersistentPool &pool) const;
    void load(PersistentPool &pool);

    QString moduleName;
    QString propertyName;
    QVariant value;
    Kind kind;
};

inline bool operator==(const Property &p1, const Property &p2)
{
    return p1.moduleName == p2.moduleName && p1.propertyName == p2.propertyName;
}
bool operator<(const Property &p1, const Property &p2);

inline uint qHash(const Property &p)
{
    return QT_PREPEND_NAMESPACE(qHash)(p.moduleName + p.propertyName);
}

typedef Set<Property> PropertySet;
typedef QHash<QString, PropertySet> PropertyHash;

} // namespace Internal
} // namespace qbs

#endif // Include guard
