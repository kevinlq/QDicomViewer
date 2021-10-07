/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing
**
** This file is part of Qbs.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms and
** conditions see http://www.qt.io/terms-conditions. For further information
** use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file.  Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, The Qt Company gives you certain additional
** rights.  These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#ifndef QBS_VISUALSTUDIOGENERATOR_H
#define QBS_VISUALSTUDIOGENERATOR_H

#include <generators/generator.h>
#include <generators/igeneratableprojectvisitor.h>
#include <tools/visualstudioversioninfo.h>
#include "visualstudioguidpool.h"

#include <QtCore/qfileinfo.h>
#include <QtCore/qdir.h>
#include <QtCore/qmap.h>

namespace qbs {

namespace Internal { class VisualStudioVersionInfo; }

class MSBuildProject;
class MSBuildTargetProject;

class VisualStudioGeneratorPrivate;
class VisualStudioSolution;
class VisualStudioSolutionFileProject;
class VisualStudioSolutionFolderProject;

class VisualStudioGenerator : public ProjectGenerator, private IGeneratableProjectVisitor
{
    friend class SolutionDependenciesVisitor;
public:
    explicit VisualStudioGenerator(const Internal::VisualStudioVersionInfo &versionInfo);
    ~VisualStudioGenerator();
    QString generatorName() const override;
    void generate() override;

private:
    virtual void visitProject(const GeneratableProject &project) override;
    virtual void visitProjectData(const GeneratableProject &project,
                                  const GeneratableProjectData &projectData) override;
    virtual void visitProduct(const GeneratableProject &project,
                              const GeneratableProjectData &projectData,
                              const GeneratableProductData &productData) override;

    void addPropertySheets(const GeneratableProject &project);
    void addPropertySheets(const std::shared_ptr<MSBuildTargetProject> &targetProject);

    QScopedPointer<VisualStudioGeneratorPrivate> d;
};

} // namespace qbs

#endif // QBS_VISUALSTUDIOGENERATOR_H
