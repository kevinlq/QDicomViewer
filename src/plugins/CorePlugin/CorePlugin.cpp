#include "CorePlugin.h"
#include "MainWindow.h"
#include "CoreInclude.h"

#include <extensionsystem/pluginerroroverview.h>
#include <extensionsystem/pluginmanager.h>
#include <extensionsystem/pluginspec.h>
#include <utils/algorithm.h>
#include <utils/pathchooser.h>
#include <utils/macroexpander.h>
#include <utils/mimetypes/mimedatabase.h>
#include <utils/savefile.h>
#include <utils/stringutils.h>
#include <utils/theme/theme.h>
#include <utils/theme/theme_p.h>

#include "ListPanel/QListPanelManager.h"
#include "ToolPanel/QToolPanelManager.h"

#include "actionmanager/actionmanager.h"
#include "themechooser.h"
#include "id.h"

using namespace Core;
using namespace Core::Internal;
using namespace Utils;

struct CoreArguments {
    QColor overrideColor;
    Id themeId;
    bool presentationMode = false;
};

CoreArguments parseArguments(const QStringList &arguments)
{
    CoreArguments args;
    for (int i = 0; i < arguments.size(); ++i) {
        if (arguments.at(i) == QLatin1String("-color")) {
            const QString colorcode(arguments.at(i + 1));
            args.overrideColor = QColor(colorcode);
            i++; // skip the argument
        }
        if (arguments.at(i) == QLatin1String("-presentationMode"))
            args.presentationMode = true;
        if (arguments.at(i) == QLatin1String("-theme")) {
            args.themeId = Id::fromString(arguments.at(i + 1));
            i++; // skip the argument
        }
    }
    return args;
}

CorePlugin::CorePlugin()
    : m_pMainWindow(nullptr)
{
    qRegisterMetaType<Id>();
}

CorePlugin::~CorePlugin()
{
    if (nullptr != m_pMainWindow)
    {
        delete m_pMainWindow;
        m_pMainWindow = nullptr;
    }

    setCreatorTheme(nullptr);
}

bool CorePlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments);
    // register all mime types from all plugins
    for (ExtensionSystem::PluginSpec *plugin : ExtensionSystem::PluginManager::plugins())
    {
        if (!plugin->isEffectivelyEnabled())
        {
            continue;
        }

        const QJsonObject metaData = plugin->metaData();
        const QJsonValue mimetypes = metaData.value("Mimetypes");
        QString mimetypeString;

        if (Utils::readMultiLineString(mimetypes, &mimetypeString))
        {
            Utils::addMimeTypes(plugin->name() + ".mimetypes", mimetypeString.trimmed().toUtf8());
        }
    }

    if (ThemeEntry::availableThemes().isEmpty()) {
        *errorMessage = tr("No themes found in installation.");
        return false;
    }
    const CoreArguments args = parseArguments(arguments);
    Theme::initialPalette(); // Initialize palette before setting it
    Theme *themeFromArg = ThemeEntry::createTheme(args.themeId);
    setCreatorTheme(themeFromArg ? themeFromArg
                                 : ThemeEntry::createTheme(ThemeEntry::themeSetting()));

    new QListPanelManager(this);

    new ActionManager(this);
    ActionManager::setPresentationModeEnabled(args.presentationMode);

    m_pMainWindow = new(std::nothrow) MainWindow;
    qsrand(QDateTime::currentDateTime().toTime_t());

    const bool success = m_pMainWindow->init(errorMessage);
    if (success)
    {
    }

    //IWizardFactory::initialize();

    return success;
}

void CorePlugin::extensionsInitialized()
{
    //
    m_pMainWindow->extensionsInitialized();
}

bool CorePlugin::delayedInitialize()
{
    return true;
}

ExtensionSystem::IPlugin::ShutdownFlag CorePlugin::aboutToShutdown()
{
    //Find::aboutToShutdown();
    //m_mainWindow->aboutToShutdown();
    return SynchronousShutdown;
}

QObject *CorePlugin::remoteCommand(const QStringList &, const QString &workingDirectory, const QStringList &args)
{
    if (!ExtensionSystem::PluginManager::isInitializationDone()) {
        connect(ExtensionSystem::PluginManager::instance(), &ExtensionSystem::PluginManager::initializationDone,
                this, [this, workingDirectory, args]() {
                    remoteCommand(QStringList(), workingDirectory, args);
        });
        return nullptr;
    }
//    IDocument *res = m_mainWindow->openFiles(
//                args, ICore::OpenFilesFlags(ICore::SwitchMode | ICore::CanContainLineAndColumnNumbers),
//                workingDirectory);
//    m_mainWindow->raiseWindow();
    return nullptr;
}

void CorePlugin::fileOpenRequest(const QString &f)
{
    remoteCommand(QStringList(), QString(), QStringList(f));
}
