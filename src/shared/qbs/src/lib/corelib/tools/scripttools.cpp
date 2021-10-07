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

#include "scripttools.h"

#include <QtCore/qdatastream.h>

#include <QtScript/qscriptengine.h>

namespace qbs {
namespace Internal {

void setConfigProperty(QVariantMap &cfg, const QStringList &name, const QVariant &value)
{
    if (name.length() == 1) {
        cfg.insert(name.first(), value);
    } else {
        QVariant &subCfg = cfg[name.first()];
        QVariantMap subCfgMap = subCfg.toMap();
        setConfigProperty(subCfgMap, name.mid(1), value);
        subCfg = subCfgMap;
    }
}

QVariant getConfigProperty(const QVariantMap &cfg, const QStringList &name)
{
    if (name.length() == 1)
        return cfg.value(name.first());
    else
        return getConfigProperty(cfg.value(name.first()).toMap(), name.mid(1));
}

TemporaryGlobalObjectSetter::TemporaryGlobalObjectSetter(const QScriptValue &object)
{
    QScriptEngine *engine = object.engine();
    m_oldGlobalObject = engine->globalObject();
    engine->setGlobalObject(object);
}

TemporaryGlobalObjectSetter::~TemporaryGlobalObjectSetter()
{
    m_oldGlobalObject.engine()->setGlobalObject(m_oldGlobalObject);
}

} // namespace Internal
} // namespace qbs
