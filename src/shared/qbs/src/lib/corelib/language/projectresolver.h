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

#ifndef PROJECTRESOLVER_H
#define PROJECTRESOLVER_H

#include "filetags.h"
#include "itemtype.h"
#include "moduleloader.h"
#include "qualifiedid.h"

#include <logging/logger.h>
#include <tools/set.h>

#include <QtCore/qhash.h>
#include <QtCore/qmap.h>
#include <QtCore/qstringlist.h>

#include <utility>

namespace qbs {
namespace Internal {

class Evaluator;
class Item;
class ProgressObserver;
class ScriptEngine;

class ProjectResolver
{
public:
    ProjectResolver(Evaluator *evaluator, const ModuleLoaderResult &loadResult,
                    const SetupProjectParameters &setupParameters, Logger &logger);
    ~ProjectResolver();

    void setProgressObserver(ProgressObserver *observer);
    TopLevelProjectPtr resolve();

    static void applyFileTaggers(const SourceArtifactPtr &artifact,
            const ResolvedProductConstPtr &product, Logger &logger);

    static SourceArtifactPtr createSourceArtifact(const ResolvedProductPtr &rproduct,
            const QString &fileName, const GroupPtr &group, bool wildcard,
            const CodeLocation &filesLocation = CodeLocation(),
            QHash<QString, CodeLocation> *fileLocations = nullptr, ErrorInfo *errorInfo = nullptr);

private:
    struct ProjectContext;
    struct ProductContext;
    struct ModuleContext;

    void checkCancelation() const;
    QString verbatimValue(const ValueConstPtr &value, bool *propertyWasSet = 0) const;
    QString verbatimValue(Item *item, const QString &name, bool *propertyWasSet = 0) const;
    ScriptFunctionPtr scriptFunctionValue(Item *item, const QString &name) const;
    ResolvedFileContextPtr resolvedFileContext(const FileContextConstPtr &ctx) const;
    void ignoreItem(Item *item, ProjectContext *projectContext);
    TopLevelProjectPtr resolveTopLevelProject();
    void resolveProject(Item *item, ProjectContext *projectContext);
    void resolveSubProject(Item *item, ProjectContext *projectContext);
    void resolveProduct(Item *item, ProjectContext *projectContext);
    void resolveModules(const Item *item, ProjectContext *projectContext);
    void resolveModule(const QualifiedId &moduleName, Item *item, bool isProduct,
                       const QVariantMap &parameters, ProjectContext *projectContext);
    QVariantMap resolveAdditionalModuleProperties(const Item *group,
                                                  const QVariantMap &currentValues);
    void resolveGroup(Item *item, ProjectContext *projectContext);
    void resolveRule(Item *item, ProjectContext *projectContext);
    void resolveRuleArtifact(const RulePtr &rule, Item *item);
    void resolveRuleArtifactBinding(const RuleArtifactPtr &ruleArtifact, Item *item,
                                    const QStringList &namePrefix,
                                    QualifiedIdSet *seenBindings);
    void resolveFileTagger(Item *item, ProjectContext *projectContext);
    void resolveScanner(Item *item, ProjectContext *projectContext);
    void resolveProductDependencies(const ProjectContext &projectContext);
    void postProcess(const ResolvedProductPtr &product, ProjectContext *projectContext) const;
    void applyFileTaggers(const ResolvedProductPtr &product) const;
    QVariantMap evaluateModuleValues(Item *item, bool lookupPrototype = true);
    QVariantMap evaluateProperties(Item *item, bool lookupPrototype = true);
    QVariantMap evaluateProperties(const Item *item, const Item *propertiesContainer,
                                   const QVariantMap &tmplt, bool lookupPrototype = true);
    QVariantMap createProductConfig();
    QString convertPathProperty(const QString &path, const QString &dirPath) const;
    QStringList convertPathListProperty(const QStringList &paths, const QString &dirPath) const;
    ProjectContext createProjectContext(ProjectContext *parentProjectContext) const;

    struct ProductDependencyInfo
    {
        ProductDependencyInfo(const ResolvedProductPtr &product,
                              const QVariantMap &parameters = QVariantMap())
            : product(product), parameters(parameters)
        {
        }

        ResolvedProductPtr product;
        QVariantMap parameters;
    };

    struct ProductDependencyInfos
    {
        std::vector<ProductDependencyInfo> dependencies;
        bool hasDisabledDependency = false;
    };

    ProductDependencyInfos getProductDependencies(const ResolvedProductConstPtr &product,
            const ModuleLoaderResult::ProductInfo &productInfo);
    QString sourceCodeAsFunction(const JSSourceValueConstPtr &value,
                                 const PropertyDeclaration &decl) const;
    QString sourceCodeForEvaluation(const JSSourceValueConstPtr &value) const;
    static void matchArtifactProperties(const ResolvedProductPtr &product,
            const QList<SourceArtifactPtr> &artifacts);
    void printProfilingInfo();
    void handleError(const ErrorInfo &error);

    Evaluator *m_evaluator;
    Logger &m_logger;
    ScriptEngine *m_engine;
    ProgressObserver *m_progressObserver;
    ProductContext *m_productContext;
    ModuleContext *m_moduleContext;
    QMap<QString, ResolvedProductPtr> m_productsByName;
    QHash<FileTag, QList<ResolvedProductPtr> > m_productsByType;
    QHash<ResolvedProductPtr, Item *> m_productItemMap;
    mutable QHash<FileContextConstPtr, ResolvedFileContextPtr> m_fileContextMap;
    mutable QHash<std::pair<QStringRef, QStringList>, QString> m_scriptFunctions;
    mutable QHash<QStringRef, QString> m_sourceCode;
    const SetupProjectParameters &m_setupParams;
    ModuleLoaderResult m_loadResult;
    Set<CodeLocation> m_groupLocationWarnings;
    qint64 m_elapsedTimeModPropEval;
    qint64 m_elapsedTimeAllPropEval;
    qint64 m_elapsedTimeGroups;

    typedef void (ProjectResolver::*ItemFuncPtr)(Item *item, ProjectContext *projectContext);
    typedef QMap<ItemType, ItemFuncPtr> ItemFuncMap;
    void callItemFunction(const ItemFuncMap &mappings, Item *item, ProjectContext *projectContext);
};

} // namespace Internal
} // namespace qbs

#endif // PROJECTRESOLVER_H
