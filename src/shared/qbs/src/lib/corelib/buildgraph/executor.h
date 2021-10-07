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

#ifndef QBS_BUILDGRAPHEXECUTOR_H
#define QBS_BUILDGRAPHEXECUTOR_H

#include "forward_decls.h"
#include "buildgraphvisitor.h"
#include <buildgraph/artifact.h>
#include <language/forward_decls.h>

#include <logging/logger.h>
#include <tools/buildoptions.h>
#include <tools/error.h>

#include <QtCore/qobject.h>
#include <queue>

QT_BEGIN_NAMESPACE
class QTimer;
QT_END_NAMESPACE

namespace qbs {
class ProcessResult;

namespace Internal {
class ExecutorJob;
class FileTime;
class InputArtifactScannerContext;
class ProductInstaller;
class ProgressObserver;
class RuleNode;

class Executor : public QObject, private BuildGraphVisitor
{
    Q_OBJECT

public:
    void build();

    Executor(const Logger &logger, QObject *parent = 0);
    ~Executor();

    void setProject(const TopLevelProjectPtr &project);
    void setProducts(const QList<ResolvedProductPtr> &productsToBuild);
    void setBuildOptions(const BuildOptions &buildOptions);
    void setProgressObserver(ProgressObserver *observer) { m_progressObserver = observer; }

    ErrorInfo error() const { return m_error; }

signals:
    void reportCommandDescription(const QString &highlight, const QString &message);
    void reportProcessResult(const qbs::ProcessResult &result);

    void finished();

private:
    void onJobFinished(const qbs::ErrorInfo &err);
    void finish();
    void checkForCancellation();

    // BuildGraphVisitor implementation
    bool visit(Artifact *artifact);
    bool visit(RuleNode *ruleNode);

    enum ExecutorState { ExecutorIdle, ExecutorRunning, ExecutorCanceling };

    struct ComparePriority
    {
        bool operator() (const BuildGraphNode *x, const BuildGraphNode *y) const;
    };

    typedef std::priority_queue<BuildGraphNode *, std::vector<BuildGraphNode *>,
                                ComparePriority> Leaves;

    void doBuild();
    void prepareAllNodes();
    void prepareArtifact(Artifact *artifact);
    void setupForBuildingSelectedFiles(const BuildGraphNode *node);
    void prepareReachableNodes();
    void prepareReachableNodes_impl(BuildGraphNode *node);
    void prepareProducts();
    void setupRootNodes();
    void initLeaves();
    void updateLeaves(const NodeSet &nodes);
    void updateLeaves(BuildGraphNode *node, NodeSet &seenNodes);
    bool scheduleJobs();
    void buildArtifact(Artifact *artifact);
    void executeRuleNode(RuleNode *ruleNode);
    void finishJob(ExecutorJob *job, bool success);
    void finishNode(BuildGraphNode *leaf);
    void finishArtifact(Artifact *artifact);
    void setState(ExecutorState);
    void addExecutorJobs();
    void cancelJobs();
    void setupProgressObserver();
    void doSanityChecks();
    void handleError(const ErrorInfo &error);
    void rescueOldBuildData(Artifact *artifact, bool *childrenAdded);
    bool checkForUnbuiltDependencies(Artifact *artifact);
    void potentiallyRunTransformer(const TransformerPtr &transformer);
    void runTransformer(const TransformerPtr &transformer);
    void finishTransformer(const TransformerPtr &transformer);
    void possiblyInstallArtifact(const Artifact *artifact);
    void checkForUnbuiltProducts();
    bool checkNodeProduct(BuildGraphNode *node);

    bool mustExecuteTransformer(const TransformerPtr &transformer) const;
    bool isUpToDate(Artifact *artifact) const;
    void retrieveSourceFileTimestamp(Artifact *artifact) const;
    FileTime recursiveFileTime(const QString &filePath) const;
    QString configString() const;
    bool transformerHasMatchingOutputTags(const TransformerConstPtr &transformer) const;
    bool artifactHasMatchingOutputTags(const Artifact *artifact) const;
    bool transformerHasMatchingInputFiles(const TransformerConstPtr &transformer) const;

    typedef QHash<ExecutorJob *, TransformerPtr> JobMap;
    JobMap m_processingJobs;

    ProductInstaller *m_productInstaller;
    RulesEvaluationContextPtr m_evalContext;
    BuildOptions m_buildOptions;
    Logger m_logger;
    ProgressObserver *m_progressObserver;
    QList<ExecutorJob*> m_availableJobs;
    ExecutorState m_state;
    TopLevelProjectPtr m_project;
    QList<ResolvedProductPtr> m_productsToBuild;
    NodeSet m_roots;
    Leaves m_leaves;
    QList<Artifact *> m_changedSourceArtifacts;
    InputArtifactScannerContext *m_inputArtifactScanContext;
    ErrorInfo m_error;
    bool m_explicitlyCanceled;
    FileTags m_activeFileTags;
    FileTags m_tagsOfFilesToConsider;
    FileTags m_tagsNeededForFilesToConsider;
    QList<ResolvedProductPtr> m_productsOfFilesToConsider;
    QTimer * const m_cancelationTimer;
    QStringList m_artifactsRemovedFromDisk;
    bool m_partialBuild;
    qint64 m_elapsedTimeRules;
    qint64 m_elapsedTimeScanners;
    qint64 m_elapsedTimeInstalling;
    const bool m_doTrace;
    const bool m_doDebug;
};

} // namespace Internal
} // namespace qbs

#endif // QBS_BUILDGRAPHEXECUTOR_H
