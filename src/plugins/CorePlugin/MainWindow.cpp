#include "MainWindow.h"
#include "CoreInclude.h"
#include "coreconstants.h"

#include <extensionsystem/pluginmanager.h>
#include <utils/algorithm.h>
#include <utils/historycompleter.h>
#include <utils/hostosinfo.h>
#include <utils/mimetypes/mimedatabase.h>
#include <utils/qtcassert.h>
#include <utils/stylehelper.h>
#include <utils/theme/theme.h>
#include <utils/stringutils.h>
#include <utils/utilsicons.h>
#include <utils/filesearch.h>
#include <utils/Frame/LayoutSelectPanel.h>
#include <App/app_version.h>

#include <CorePlugin/actionmanager/actioncontainer.h>
#include <CorePlugin/actionmanager/actionmanager.h>
#include <CorePlugin/actionmanager/actionmanager_p.h>
#include <CorePlugin/actionmanager/command.h>

#include "CoreUtil.h"
#include "CoreDataStruct.h"
#include "ContentWidgetContainer.h"
#include "Dialogs/plugindialog.h"
#include "Dialogs/versiondialog.h"
#include "Dialogs/settingsdialog.h"

using namespace ExtensionSystem;
using namespace Utils;

namespace Core {
namespace Internal {

enum { debugMainWindow = 0 };

static const char settingsGroup[] = "MainWindow";
static const char colorKey[] = "Color";
static const char windowGeometryKey[] = "WindowGeometry";
static const char windowStateKey[] = "WindowState";
static const char modeSelectorVisibleKey[] = "ModeSelectorVisible";


MainWindow::MainWindow()
    : AppMainWindow()
    , m_coreImpl(new ICore(this))
    , m_pContentWidget(new(std::nothrow) ContentWidgetContainer(this))
    , m_lowPrioAdditionalContexts(Constants::C_GLOBAL)
{
    setWindowTitle(tr(Constants::IDE_APP_NAME));
    QCoreApplication::setApplicationName(QLatin1String(Constants::IDE_APP_NAME));
    QCoreApplication::setApplicationVersion(QLatin1String(Constants::IDE_VERSION_LONG));
    QCoreApplication::setOrganizationName(QLatin1String(Constants::IDE_SETTINGSVARIANT_STR));

    QString baseName = QApplication::style()->objectName();

    // Sometimes we get the standard windows 95 style as a fallback
    if (HostOsInfo::isAnyUnixHost() && !HostOsInfo::isMacHost()
            && baseName == QLatin1String("windows")) {
        baseName = QLatin1String("fusion");
    }

    connect(qApp, &QApplication::focusChanged, this, &MainWindow::updateFocusWidget);

    registerDefaultContainers();
    registerDefaultActions();

    setCentralWidget(m_pContentWidget);
}

MainWindow::~MainWindow()
{
    PluginManager::removeObject(m_coreImpl);
    delete m_coreImpl;
    m_coreImpl = nullptr;

    if (Q_NULLPTR != m_pSelectPanel)
    {
        delete m_pSelectPanel;
        m_pSelectPanel = Q_NULLPTR;
    }
}

bool MainWindow::init(QString *errorMessage)
{
    Q_UNUSED(errorMessage);

    PluginManager::addObject(m_coreImpl);

    m_pContentWidget->init();

    return true;
}

void MainWindow::extensionsInitialized()
{
    emit m_coreImpl->coreAboutToOpen();

    readSettings();
    updateContext();

    // Delay restoreWindowState, since it is overridden by LayoutRequest event
    QTimer::singleShot(0, this, &MainWindow::restoreWindowState);
    QTimer::singleShot(0, m_coreImpl, &ICore::coreOpened);
}

void MainWindow::aboutToShutdown()
{
    disconnect(qApp, &QApplication::focusChanged, this, &MainWindow::updateFocusWidget);
    m_activeContext.clear();
    hide();
}

IContext *MainWindow::contextObject(QWidget *widget)
{
    return m_contextWidgets.value(widget);
}

void MainWindow::addContextObject(IContext *context)
{
    if (!context)
        return;
    QWidget *widget = context->widget();
    if (m_contextWidgets.contains(widget))
        return;

    m_contextWidgets.insert(widget, context);
}

void MainWindow::removeContextObject(IContext *context)
{
    if (!context)
        return;

    QWidget *widget = context->widget();
    if (!m_contextWidgets.contains(widget))
        return;

    m_contextWidgets.remove(widget);
    if (m_activeContext.removeAll(context) > 0)
        updateContextObject(m_activeContext);
}

IContext *MainWindow::currentContextObject() const
{
    return m_activeContext.isEmpty() ? 0 : m_activeContext.first();
}

void MainWindow::updateAdditionalContexts(const Context &remove, const Context &add,
                                          ICore::ContextPriority priority)
{
    foreach (const Id id, remove) {
        if (!id.isValid())
            continue;
        int index = m_lowPrioAdditionalContexts.indexOf(id);
        if (index != -1)
            m_lowPrioAdditionalContexts.removeAt(index);
        index = m_highPrioAdditionalContexts.indexOf(id);
        if (index != -1)
            m_highPrioAdditionalContexts.removeAt(index);
    }

    foreach (const Id id, add) {
        if (!id.isValid())
            continue;
        Context &cref = (priority == ICore::ContextPriority::High ? m_highPrioAdditionalContexts
                                                                  : m_lowPrioAdditionalContexts);
        if (!cref.contains(id))
            cref.prepend(id);
    }

    updateContext();
}

QStringList MainWindow::additionalAboutInformation() const
{
    return m_aboutInformation;
}

void MainWindow::appendAboutInformation(const QString &line)
{
    m_aboutInformation.append(line);
}

bool MainWindow::showWarningWithOptions(const QString &title,
                                        const QString &text,
                                        const QString &details,
                                        Id settingsId,
                                        QWidget *parent)
{
    if (parent == 0)
        parent = this;
    QMessageBox msgBox(QMessageBox::Warning, title, text,
                       QMessageBox::Ok, parent);
    if (!details.isEmpty())
        msgBox.setDetailedText(details);
    QAbstractButton *settingsButton = nullptr;
    if (settingsId.isValid())
        settingsButton = msgBox.addButton(tr("Settings..."), QMessageBox::AcceptRole);
    msgBox.exec();
    if (settingsButton && msgBox.clickedButton() == settingsButton)
        return showOptionsDialog(settingsId);
    return false;
}

void MainWindow::readSettings()
{
    QSettings *settings = PluginManager::settings();
    settings->beginGroup(QLatin1String(settingsGroup));

    if (m_overrideColor.isValid()) {
        StyleHelper::setBaseColor(m_overrideColor);
        // Get adapted base color.
        m_overrideColor = StyleHelper::baseColor();
    } else {
        StyleHelper::setBaseColor(settings->value(QLatin1String(colorKey),
                                  QColor(StyleHelper::DEFAULT_BASE_COLOR)).value<QColor>());
    }

    settings->endGroup();
}

void MainWindow::saveSettings()
{
    QSettings *settings = PluginManager::settings();
    settings->beginGroup(QLatin1String(settingsGroup));

    if (!(m_overrideColor.isValid() && StyleHelper::baseColor() == m_overrideColor))
        settings->setValue(QLatin1String(colorKey), StyleHelper::requestedBaseColor());

    settings->endGroup();

    ActionManager::saveSettings();
}

void MainWindow::updateFocusWidget(QWidget *old, QWidget *now)
{
    Q_UNUSED(old);
    Q_UNUSED(now);

    // Prevent changing the context object just because the menu or a menu item is activated
    if (qobject_cast<QMenuBar*>(now) || qobject_cast<QMenu*>(now))
        return;

    QList<IContext *> newContext;
    if (QWidget *p = QApplication::focusWidget()) {
        IContext *context = nullptr;
        while (p) {
            context = m_contextWidgets.value(p);
            if (context)
                newContext.append(context);
            p = p->parentWidget();
        }
    }

    // ignore toplevels that define no context, like popups without parent
    if (!newContext.isEmpty() || QApplication::focusWidget() == focusWidget())
        updateContextObject(newContext);
}

void MainWindow::restoreWindowState()
{
    QSettings *settings = PluginManager::settings();
    settings->beginGroup(QLatin1String(settingsGroup));
    if (!restoreGeometry(settings->value(QLatin1String(windowGeometryKey)).toByteArray()))
    {
        resize(900, 640); // size without window decoration
    }
    restoreState(settings->value(QLatin1String(windowStateKey)).toByteArray());
    settings->endGroup();

    this->show();
}

void MainWindow::registerDefaultContainers()
{
    ActionContainer *menubar = ActionManager::createMenuBar(Constants::MENU_BAR);

    // System menu bar on Mac
    if (!HostOsInfo::isMacHost())
    {
        setMenuBar(menubar->menuBar());
    }

    QList<const char *> lsGroup;
    lsGroup << Constants::G_ARCHIVE << Constants::G_FOLDER << Constants::G_DOWNLOAD << Constants::G_TOOLS
            << Constants::G_SYNCHRONIZE << Constants::G_SERIES_BROWSER << Constants::G_WL
            << Constants::G_PAN_IMAGE << Constants::G_ZOOM_IMAGE << Constants::G_MEASURE
            << Constants::G_ROTATE << Constants::G_CINE_PLAY << Constants::G_MULTIPLANAR << Constants::G_FUSION
            << Constants::G_HELP;

    for (int i = 0 ;i < lsGroup.size(); i++)
    {
       menubar->appendGroup(lsGroup[i]);
    }

    auto initMenuGroup = [&menubar](ActionContainer *pMenu, Id groupId, QList<const char *> lsGroup)->bool
    {

        if (Q_NULLPTR == pMenu)
        {
            return false;
        }

        menubar->addMenu(pMenu, groupId);

        for (int i = 0; i < lsGroup.size(); i++)
        {
            pMenu->appendGroup(lsGroup[i]);
        }

        return true;
    };

    // 本地数据库
    ActionContainer *pArchiveMenu = ActionManager::createMenu(Constants::M_ARCHIVE);
    pArchiveMenu->menu()->setTitle(tr("&Archive"));
    lsGroup = {Constants::G_ARCHIVE_IMPORT, Constants::G_ARCHIVE_EXIT};
    initMenuGroup(pArchiveMenu, Constants::G_ARCHIVE, lsGroup);

    // 打开文件
    ActionContainer *pFolderMenu = ActionManager::createMenu(Constants::M_FOLDER);
    pFolderMenu->menu()->setTitle(tr("&Folder"));
    lsGroup = {Constants::G_FOLDER_NEW, Constants::G_FOLDER_COPY,Constants::G_FOLDER_OPENFOLDER
               ,Constants::G_FOLDER_OPENFILE,Constants::G_FOLDER_ZIP, Constants::G_FOLDER_PKG};
    initMenuGroup(pFolderMenu, Constants::G_FOLDER, lsGroup);

    // 下载
    ActionContainer* pDownloadMenu = ActionManager::createMenu(Constants::M_DOWNLOAD);
    pDownloadMenu->menu()->setTitle(tr("&Download"));
    lsGroup = {Constants::G_DOWNLOAD_FIND,Constants::G_DOWNLOAD_ACCEPT};
    initMenuGroup(pDownloadMenu, Constants::G_DOWNLOAD, lsGroup);

    // 工具菜单
    ActionContainer* pToolsMenu = ActionManager::createMenu(Constants::M_TOOLS);
    pToolsMenu->menu()->setTitle(tr("&Tools"));
    lsGroup = {Constants::G_TOOLS_IMAGE,Constants::G_TOOLS_CPCLIPBOARD,Constants::G_TOOLS_CPALLCLIPBOARD
               ,Constants::G_TOOLS_SPLIT_SERIES,Constants::G_TOOLS_SHOWTAG,Constants::G_TOOLS_ANNOTATION
               ,Constants::G_TOOLS_CROSSLINE,Constants::G_TOOLS_HIDEPATIENT
               ,Constants::G_TOOLS_HIDEMEASUREMENT};
    initMenuGroup(pToolsMenu, Constants::G_TOOLS, lsGroup);

    // 窗宽窗位
    ActionContainer* pWLMenu = ActionManager::createMenu(Constants::M_WL);
    pWLMenu->menu()->setTitle(tr("&WL"));
    lsGroup = {Constants::G_WL,Constants::G_WL_DEFAULT,Constants::G_WL_FULL_DYNAMIC,Constants::G_WL_FULL_CT_ABDOMEN
               ,Constants::G_WL_FULL_CT_ANGIO,Constants::G_WL_FULL_CT_BONE,Constants::G_WL_FULL_CT_BRAIN,Constants::G_WL_FULL_CT_CHEST
              ,Constants::G_WL_FULL_CT_LUNGS,Constants::G_WL_FULL_CT_NEGATIVE};
    initMenuGroup(pWLMenu, Constants::G_WL, lsGroup);

    // 测量
    ActionContainer* pMeasureMenu = ActionManager::createMenu(Constants::M_MEASURE);
    pMeasureMenu->menu()->setTitle(tr("&Measure"));
    lsGroup = {Constants::G_MEASURE_LENGTH,Constants::G_MEASURE_ELIPSE,Constants::G_MEASURE_CLOSED_POLYGON,Constants::G_MEASURE_OPEN_POLYGON
               ,Constants::G_MEASURE_ANGLE,Constants::G_MEASURE_COBBANGLE,Constants::G_MEASURE_DEVIATION,Constants::G_MEASURE_ARROW
              ,Constants::G_MEASURE_PENCIL,Constants::G_MEASURE_3DCURSOR,Constants::G_MEASURE_EDITMEASUREMENTS,Constants::G_MEASURE_ADVANCEDTOOLS};
    initMenuGroup(pMeasureMenu, Constants::G_MEASURE, lsGroup);

    // 旋转
    ActionContainer *pRogateMenu = ActionManager::createMenu(Constants::M_ROTATE);
    pRogateMenu->menu()->setTitle(tr("&Rotate"));
    lsGroup = {Constants::G_ROTATE_CCW,Constants::G_ROTATE_CW,Constants::G_ROTATE_180, Constants::G_ROTATE_HFLIP
               , Constants::G_ROTATE_VFLIP, Constants::G_ROTATE_CLEAR};
    initMenuGroup(pRogateMenu, Constants::G_ROTATE, lsGroup);

    // 帮助
    ActionContainer* pHelp = ActionManager::createMenu(Constants::M_HELP);
    pHelp->menu()->setTitle(tr("&Help"));
    lsGroup = {Constants::G_HELP_ONLINE,Constants::G_HELP_CHANGE_LANGUAGE,Constants::G_HELP_CHANGE_CUSTOMIZE_KEY
               ,Constants::G_HELP_QRADIANT_ONLINE, Constants::G_HELP_ABOUT_QDV, Constants::G_HELP_ABOUT_PLUGIN};
    initMenuGroup(pHelp, Constants::G_HELP, lsGroup);
}

void MainWindow::registerDefaultActions()
{
    ActionContainer *pArchiveMenu   = ActionManager::actionContainer(Constants::M_ARCHIVE);
    ActionContainer *pFolderMenu    = ActionManager::actionContainer(Constants::M_FOLDER);
    ActionContainer *pToolsMenu     = ActionManager::actionContainer(Constants::M_TOOLS);
    ActionContainer *pHelpMenu      = ActionManager::actionContainer(Constants::M_HELP);

    // ========================= Import =========================
    QAction *tmpaction = new QAction(tr("&import"), this);
    Command *cmd = ActionManager::registerAction(tmpaction, Constants::IMPORT);
    pArchiveMenu->addAction(cmd, Constants::G_ARCHIVE_IMPORT);

    // exit
    tmpaction = new QAction(tr("exit"), this);
    cmd = ActionManager::registerAction(tmpaction, Constants::EXIT);
    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Alt+F4")));
    pArchiveMenu->addAction(cmd, Constants::G_ARCHIVE_EXIT);
    pArchiveMenu->menu()->setEnabled(false);

    // ========================= Folder =========================
    tmpaction  = new QAction(tr("&Open Dicom Folder"), this);
    cmd = ActionManager::registerAction(tmpaction, Constants::HELP_FOLDER_OPEN_FOLDER);
    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Shift+O")));
    pFolderMenu->addAction(cmd, Constants::G_FOLDER_OPENFOLDER);
    connect(tmpaction, &QAction::triggered, this, &MainWindow::openDcmFolder);

    tmpaction  = new QAction(tr("&Open Dicom File"), this);
    cmd = ActionManager::registerAction(tmpaction, Constants::HELP_FOLDER_OPEN_FILE);
    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+O")));
    pFolderMenu->addAction(cmd, Constants::G_FOLDER_OPENFILE);
    connect(tmpaction, &QAction::triggered, this, &MainWindow::openDcmFile);

    tmpaction  = new QAction(tr("&Open Dicom Zip"), this);
    cmd = ActionManager::registerAction(tmpaction, Constants::HELP_FOLDER_OPEN_ZIPFILE);
    pFolderMenu->addAction(cmd, Constants::G_FOLDER_ZIP);
    connect(tmpaction, &QAction::triggered, this, &MainWindow::openDcmZipFile);

    tmpaction  = new QAction(tr("&Open Dicom PKG"), this);
    cmd = ActionManager::registerAction(tmpaction, Constants::HELP_FOLDER_OPEN_PKGFILE);
    pFolderMenu->addAction(cmd, Constants::G_FOLDER_PKG);
    connect(tmpaction, &QAction::triggered, this, &MainWindow::openDcmPKGFile);

    //=========================  工具栏========================
    tmpaction  = new QAction(tr("&Export To Image"), this);
    cmd = ActionManager::registerAction(tmpaction, Constants::EXPORT_TO_IMAGE);
    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+E")));
    pToolsMenu->addAction(cmd, Constants::G_TOOLS_IMAGE);

    tmpaction  = new QAction(tr("&Export To Clipboard"), this);
    cmd = ActionManager::registerAction(tmpaction, Constants::EXPORT_TO_CLIPBOARD);
    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+C")));
    pToolsMenu->addAction(cmd, Constants::G_TOOLS_CPCLIPBOARD);

    pToolsMenu->addSeparator(Constants::G_TOOLS_CPCLIPBOARD);

    m_pSelectPanel = new LayoutSelectPanel(this);
    connect(m_pSelectPanel, &LayoutSelectPanel::signalSelectLayoutChange, m_pContentWidget, &ContentWidgetContainer::updateLayout);
    QWidgetAction *pWidgetAction  = new QWidgetAction(this/*tr("&Split"), this*/);
    pWidgetAction->setText(tr("&Split"));
    pWidgetAction->setDefaultWidget(m_pSelectPanel);
    cmd = ActionManager::registerAction(pWidgetAction, Constants::TOOLS_SPLIT);
    pToolsMenu->addAction(cmd, Constants::G_TOOLS_SPLIT_SERIES);
    connect(pWidgetAction, &QWidgetAction::triggered, this, &MainWindow::showSplitPanel);

    tmpaction  = new QAction(tr("&Show Annotation"), this);
    tmpaction->setCheckable(true);
    tmpaction->setChecked(true);
    cmd = ActionManager::registerAction(tmpaction, Constants::TOOLS_SHOWANNO);
    cmd->setDefaultKeySequence(QKeySequence(Qt::Key_F12));
    pToolsMenu->addAction(cmd, Constants::G_TOOLS_ANNOTATION);

    tmpaction  = new QAction(tr("&Show CrossLine"), this);
    tmpaction->setCheckable(true);
    tmpaction->setChecked(true);
    cmd = ActionManager::registerAction(tmpaction, Constants::TOOLS_SHOWCROSSLINE);
    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+F12")));
    pToolsMenu->addAction(cmd, Constants::G_TOOLS_CROSSLINE);

    tmpaction  = new QAction(tr("&Hide Patient"), this);
    tmpaction->setCheckable(true);
    cmd = ActionManager::registerAction(tmpaction, Constants::TOOLS_HIDEPATIENT);
    cmd->setDefaultKeySequence(QKeySequence(tr("Shift+F12")));
    pToolsMenu->addAction(cmd, Constants::G_TOOLS_HIDEPATIENT);

    tmpaction  = new QAction(tr("&Hide Measurement"), this);
    tmpaction->setCheckable(true);
    cmd = ActionManager::registerAction(tmpaction, Constants::TOOLS_HIDEMEASUREMENT);
    cmd->setDefaultKeySequence(QKeySequence(tr("Alt+F12")));
    pToolsMenu->addAction(cmd, Constants::G_TOOLS_HIDEMEASUREMENT);

    tmpaction  = new QAction(tr("&Show Tag"), this);
    tmpaction->setCheckable(false);
    cmd = ActionManager::registerAction(tmpaction, Constants::TOOLS_SHOWTAG);
    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Alt+T")));
    pToolsMenu->addAction(cmd, Constants::G_TOOLS_SHOWTAG);

    //========================= Help=============================
    tmpaction  = new QAction(tr("&Online Help"), this);
    cmd = ActionManager::registerAction(tmpaction, Constants::HELP_ONLINE);
    cmd->setDefaultKeySequence(QKeySequence(Qt::Key_F1));
    pHelpMenu->addAction(cmd, Constants::G_HELP_ONLINE);

    // 切换语言菜单
    {
        ActionContainer *pLanguageMenu = ActionManager::createMenu(Constants::M_HELP_LANGUAGE);
        pHelpMenu->addMenu(pLanguageMenu, Constants::G_HELP_CHANGE_LANGUAGE);
        pLanguageMenu->menu()->setTitle(tr("&Change Language"));
        pLanguageMenu->appendGroup(Constants::G_HELP_LANGUAGE_EN);
        pLanguageMenu->appendGroup(Constants::G_HELP_LANGUAGE_CH);
        pLanguageMenu->appendGroup(Constants::G_HELP_LANGUAGE_DOWNLOAD);
        pLanguageMenu->appendGroup(Constants::G_HELP_LANGUAGE_LOAD);

        pLanguageMenu->addSeparator(Constants::G_HELP_LANGUAGE_EN);
        pLanguageMenu->addSeparator(Constants::G_HELP_LANGUAGE_CH);

        tmpaction = new QAction(tr("&Engligh"), this);
        cmd = ActionManager::registerAction(tmpaction, Constants::HELP_LANGUAGE_EN);
        pLanguageMenu->addAction(cmd, Constants::G_HELP_LANGUAGE_EN);

        tmpaction = new QAction(tr("&CN"), this);
        cmd = ActionManager::registerAction(tmpaction, Constants::HELP_LANGUAGE_CH);
        pLanguageMenu->addAction(cmd, Constants::G_HELP_LANGUAGE_CH);

        tmpaction = new QAction(tr("&Download Language file"), this);
        cmd = ActionManager::registerAction(tmpaction, Constants::HELP_LANGUAGE_DOWNLOAD);
        pLanguageMenu->addAction(cmd, Constants::G_HELP_LANGUAGE_DOWNLOAD);

        tmpaction = new QAction(tr("&Load Language file"), this);
        cmd = ActionManager::registerAction(tmpaction, Constants::HELP_LANGUAGE_LOAD);
        pLanguageMenu->addAction(cmd, Constants::G_HELP_LANGUAGE_LOAD);
    }

    tmpaction = new QAction(tr("&Custom Key"), this);
    cmd = ActionManager::registerAction(tmpaction, Constants::HELP_CUSTOMIZE_KEY);
    cmd->setDefaultKeySequence(QKeySequence::Save);
    pHelpMenu->addAction(cmd, Constants::G_HELP_CHANGE_CUSTOMIZE_KEY);

    tmpaction = new QAction(tr("&About QDicomViewer"), this);
    cmd = ActionManager::registerAction(tmpaction, Constants::HELP_ABOUT_QDV);
    pHelpMenu->addAction(cmd, Constants::G_HELP_ABOUT_QDV);
    connect(tmpaction, &QAction::triggered, this, &MainWindow::aboutQDicomViewer);

    tmpaction = new QAction(tr("&About Plugins"), this);
    tmpaction->setMenuRole(QAction::ApplicationSpecificRole);
    cmd = ActionManager::registerAction(tmpaction, Constants::HELP_ABOUT_PLUGINS);
    pHelpMenu->addAction(cmd, Constants::G_HELP_ABOUT_PLUGIN);
    connect(tmpaction, &QAction::triggered, this, &MainWindow::aboutPlugins);
}

void MainWindow::updateContext()
{
    Context contexts = m_highPrioAdditionalContexts;

    foreach (IContext *context, m_activeContext)
        contexts.add(context->context());

    contexts.add(m_lowPrioAdditionalContexts);

    Context uniquecontexts;
    for (int i = 0; i < contexts.size(); ++i) {
        const Id id = contexts.at(i);
        if (!uniquecontexts.contains(id))
            uniquecontexts.add(id);
    }

    ActionManager::setContext(uniquecontexts);
    emit m_coreImpl->contextChanged(uniquecontexts);
}

void MainWindow::destroyVersionDialog()
{
    if (Q_NULLPTR != m_pVersionDialog)
    {
        m_pVersionDialog->deleteLater();
        m_pVersionDialog = Q_NULLPTR;
    }
}

void MainWindow::aboutQDicomViewer()
{
    if (Q_NULLPTR == m_pVersionDialog)
    {
        m_pVersionDialog = new VersionDialog(this);

        connect(m_pVersionDialog, &QDialog::finished,
                this, &MainWindow::destroyVersionDialog);

        ICore::registerWindow(m_pVersionDialog, Context("Core.VersionDialog"));
        m_pVersionDialog->show();
    }
    else
    {
        ICore::raiseWindow(m_pVersionDialog);
    }
}

void MainWindow::aboutPlugins()
{
    PluginDialog dialog(this);
    dialog.exec();
}

void MainWindow::showSplitPanel()
{
    if (Q_NULLPTR == m_pSelectPanel)
    {
        return;
    }

    QCursor cursor;
    m_pSelectPanel->move(cursor.pos());
    m_pSelectPanel->show();
}

void MainWindow::openDcmFolder()
{
    QStringList filters;
    QStringList fileNames = CoreUtil::getFiles(true, filters);

    qDebug() <<"fileNames:" << fileNames;
}

void MainWindow::openDcmFile()
{
    QStringList filters = {"*.dcm"};
    QStringList fileNames = CoreUtil::getFiles(false, filters);

    qDebug() <<"fileNames:" << fileNames;
}

void MainWindow::openDcmZipFile()
{
    QStringList filters = {"*.zip"};
    QStringList fileNames = CoreUtil::getFiles(false, filters);

    qDebug() <<"fileNames:" << fileNames;
}

void MainWindow::openDcmPKGFile()
{
    QStringList filters = {"*.pkg"};
    QStringList fileNames = CoreUtil::getFiles(false, filters);

    qDebug() <<"fileNames:" << fileNames;
}

bool MainWindow::showOptionsDialog(Id page, QWidget *parent)
{
    emit m_coreImpl->optionsDialogRequested();
    if (!parent)
        parent = ICore::dialogParent();
    SettingsDialog *dialog = SettingsDialog::getSettingsDialog(parent, page);
    return dialog->execDialog();
}

void MainWindow::updateContextObject(const QList<IContext *> &context)
{
    emit m_coreImpl->contextAboutToChange(context);
    m_activeContext = context;
    updateContext();
    if (debugMainWindow) {
        qDebug() << "new context objects =" << context;
        foreach (IContext *c, context)
            qDebug() << (c ? c->widget() : 0) << (c ? c->widget()->metaObject()->className() : 0);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // work around QTBUG-43344
    static bool alreadyClosed = false;
    if (alreadyClosed) {
        event->accept();
        return;
    }

    ICore::saveSettings();

    emit m_coreImpl->coreAboutToClose();

    event->accept();
    alreadyClosed = true;
}

} // namespace Internal
} // namespace Core
