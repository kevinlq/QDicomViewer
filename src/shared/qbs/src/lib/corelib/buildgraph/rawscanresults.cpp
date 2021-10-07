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

#include "rawscanresults.h"

#include "filedependency.h"
#include "depscanner.h"

#include <tools/persistence.h>
#include <language/propertymapinternal.h>

#include <utility>

namespace qbs {
namespace Internal {

void RawScanResult::load(PersistentPool &pool)
{
    pool.load(deps);
    pool.load(additionalFileTags);
}

void RawScanResult::store(PersistentPool &pool) const
{
    pool.store(deps);
    pool.store(additionalFileTags);
}

void RawScanResults::ScanData::load(PersistentPool &pool)
{
    pool.load(scannerId);
    pool.load(moduleProperties);
    pool.load(lastScanTime);
    pool.load(rawScanResult);
}

void RawScanResults::ScanData::store(PersistentPool &pool) const
{
    pool.store(scannerId);
    pool.store(moduleProperties);
    pool.store(lastScanTime);
    pool.store(rawScanResult);
}

RawScanResults::ScanData &RawScanResults::findScanData(
        const FileResourceBase *file,
        const DependencyScanner *scanner,
        const PropertyMapConstPtr &moduleProperties)
{
    std::vector<ScanData> &scanDataForFile = m_rawScanData[file->filePath()];
    const QString &scannerId = scanner->id();
    for (auto it = scanDataForFile.begin(); it != scanDataForFile.end(); ++it) {
        ScanData &scanData = *it;
        if (scannerId != scanData.scannerId)
            continue;
        if (!scanner->areModulePropertiesCompatible(moduleProperties, scanData.moduleProperties))
            continue;
        return scanData;
    }
    ScanData newScanData;
    newScanData.scannerId = scannerId;
    newScanData.moduleProperties = moduleProperties;
    scanDataForFile.push_back(std::move(newScanData));
    return scanDataForFile.back();
}

void RawScanResults::load(PersistentPool &pool)
{
    pool.load(m_rawScanData);
}

void RawScanResults::store(PersistentPool &pool) const
{
    pool.store(m_rawScanData);
}

} // namespace Internal
} // namespace qbs
