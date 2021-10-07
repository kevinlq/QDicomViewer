/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qbs.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
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
** $QT_END_LICENSE$
**
****************************************************************************/
#include "benchmarker.h"

#include "exception.h"
#include "runsupport.h"
#include "valgrindrunner.h"

#include <QtConcurrent/qtconcurrentrun.h>

#include <iostream>

namespace qbsBenchmarker {

Benchmarker::Benchmarker(Activities activities, const QString &oldCommit, const QString &newCommit,
                         const QString &testProject, const QString &qbsRepo)
    : m_activities(activities)
    , m_oldCommit(oldCommit)
    , m_newCommit(newCommit)
    , m_testProject(testProject)
    , m_qbsRepo(qbsRepo)
{
}

Benchmarker::~Benchmarker()
{
    if (!m_commitToRestore.isEmpty()) {
        try {
            runProcess(QStringList() << "git" << "checkout" << m_commitToRestore, m_qbsRepo);
        } catch (const Exception &e) {
            qDebug("Failed to restore original commit %s: %s", qPrintable(m_commitToRestore),
                   qPrintable(e.description()));
        }
    }
}

void Benchmarker::benchmark()
{
    rememberCurrentRepoState();
    runProcess(QStringList() << "git" << "checkout" << m_oldCommit, m_qbsRepo);
    const QString oldQbsBuildDir = m_baseOutputDir.path() + "/old-qbs-build";
    std::cout << "Building from old repo state..." << std::endl;
    buildQbs(oldQbsBuildDir);
    runProcess(QStringList() << "git" << "checkout" << m_newCommit, m_qbsRepo);
    const QString newQbsBuildDir = m_baseOutputDir.path() + "/new-qbs-build";
    std::cout << "Building from new repo state..." << std::endl;
    buildQbs(newQbsBuildDir);
    std::cout << "Now running valgrind. This can take a while." << std::endl;

    ValgrindRunner oldDataRetriever(m_activities, m_testProject, oldQbsBuildDir,
                                    m_baseOutputDir.path() + "/old-stuff");
    ValgrindRunner newDataRetriever(m_activities, m_testProject, newQbsBuildDir,
                                    m_baseOutputDir.path() + "/new-stuff");
    QFuture<void> oldFuture = QtConcurrent::run(&oldDataRetriever, &ValgrindRunner::run);
    QFuture<void> newFuture = QtConcurrent::run(&newDataRetriever, &ValgrindRunner::run);
    oldFuture.waitForFinished();
    foreach (const ValgrindResult &valgrindResult, oldDataRetriever.results()) {
        BenchmarkResult &benchmarkResult = m_results[valgrindResult.activity];
        benchmarkResult.oldInstructionCount = valgrindResult.instructionCount;
        benchmarkResult.oldPeakMemoryUsage = valgrindResult.peakMemoryUsage;
    }
    newFuture.waitForFinished();
    foreach (const ValgrindResult &valgrindResult, newDataRetriever.results()) {
        BenchmarkResult &benchmarkResult = m_results[valgrindResult.activity];
        benchmarkResult.newInstructionCount = valgrindResult.instructionCount;
        benchmarkResult.newPeakMemoryUsage = valgrindResult.peakMemoryUsage;
    }
    std::cout << "Done!" << std::endl;
}

void Benchmarker::rememberCurrentRepoState()
{
    QByteArray commit;
    int exitCode = 0;
    try {
        runProcess(QStringList() << "git" << "symbolic-ref" << "--short" << "HEAD", m_qbsRepo,
                   &commit, &exitCode);
    } catch (const Exception &) {
        if (exitCode == 0) {
            // runProcess did not throw because of the exit code.
            throw;
        }
        // Fallback, in case git cannot retrieve a nice symbolic name.
        runProcess(QStringList() << "git" << "describe" << "HEAD", m_qbsRepo, &commit);
    }
    m_commitToRestore = QString::fromLatin1(commit);
}

void Benchmarker::buildQbs(const QString &buildDir) const
{
    if (!QDir::root().mkpath(buildDir))
        throw Exception(QString::fromLatin1("Failed to create directory '%1'.").arg(buildDir));
    runProcess(QStringList() << "qmake" << m_qbsRepo + "/qbs.pro", buildDir);
    runProcess(QStringList() << "make" << "-s", buildDir);
}

} // namespace qbsBenchmarker
