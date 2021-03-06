#pragma once

QT_BEGIN_NAMESPACE
#include <QtGlobal>
#include <QDataStream>
#include <QSize>
#include <QColor>
QT_END_NAMESPACE

namespace Core {
namespace Constants {

const int G_STREAM_VERSION          =   QDataStream::Qt_5_3;
const char THUMBNAIL_FORMAT[]       =   "png";

const QColor BACKGROUND_COLOR(100,100,100);
const QColor LIST_PANEL_CL_NORMAL(127,127,127);
const QColor LIST_PANEL_CL_HOVER(137,137,137);
const QColor LIST_PANEL_CL_ACTIVE(197,197,197);
const QColor STATUS_PANEL_BACK_COLOR(50,50,50);
const int TOOL_PANEL_HEIGHT         = 40;
const int LIST_PANEL_WIDTH          = 150;
const int LIST_PANEL_TOOLTIP_TIME   = 2000;

const int TOOL_BUTTON_WIDTH          = 50;
const int TOOL_BUTTON_HEIGHT         = 35;
const int STATUS_PANEL_HEIGHT        = 18;

const char DROP_LIST_PANEL_KEY[]    = "qdcmView/listPanelMimedata";
const char DROP_EXTEND_KEY[]        = "text/uri-list";
const char DROP_EXTEND_FILTER[]     = ".dcm|.pkg";
const char DICOM_FILTER[]           = "*.dcm|*.pkg";

// Modes
const char MODE_WELCOME[]          = "Welcome";
const char MODE_EDIT[]             = "Edit";
const char MODE_DESIGN[]           = "Design";
const int  P_MODE_WELCOME          = 100;
const int  P_MODE_EDIT             = 90;
const int  P_MODE_DESIGN           = 89;

const char C_SETTING_DATABASE[]      = "QRadiant";

// Menubar
const char MENU_BAR[]              = "QDcmViewer.MenuBar";

// Menus

// Menus
const char M_WINDOW[]              = "QDcmViewer.Menu.Window";

const char M_MENU_BASE[]           = "QDcmViewer.Menu.";
const char M_ARCHIVE[]             = "QDcmViewer.Menu.Archive";
const char M_FOLDER[]              = "QDcmViewer.Menu.Folder";
const char M_DOWNLOAD[]            = "QDcmViewer.Menu.Download";
const char M_EXPORT[]              = "QDcmViewer.Menu.Export";
const char M_SPLIT[]               = "QDcmViewer.Menu.Split";
const char M_TOOLS[]               = "QDcmViewer.Menu.Tool";
const char M_ANNOTATION[]          = "QDcmViewer.Menu.Annotation";
const char M_WL[]                  = "QDcmViewer.Menu.WL";
const char M_MEASURE[]             = "QDcmViewer.Menu.Measure";
const char M_ROTATE[]              = "QDcmViewer.Menu.Rotate";
const char M_HELP[]                = "QDcmViewer.Menu.Help";
const char M_HELP_LANGUAGE[]       = "QDcmViewer.Menu.Language";

// Contexts
const char C_GLOBAL[]              = "Global Context";
const char C_WELCOME_MODE[]        = "Core.WelcomeMode";
const char C_EDIT_MODE[]           = "Core.EditMode";
const char C_DESIGN_MODE[]         = "Core.DesignMode";
const char C_EDITORMANAGER[]       = "Core.EditorManager";
const char C_NAVIGATION_PANE[]     = "Core.NavigationPane";
const char C_PROBLEM_PANE[]        = "Core.ProblemPane";
const char C_GENERAL_OUTPUT_PANE[] = "Core.GeneralOutputPane";

// Default editor kind
const char K_DEFAULT_TEXT_EDITOR_DISPLAY_NAME[] = QT_TRANSLATE_NOOP("OpenWith::Editors", "Plain Text Editor");
const char K_DEFAULT_TEXT_EDITOR_ID[] = "Core.PlainTextEditor";
const char K_DEFAULT_BINARY_EDITOR_ID[] = "Core.BinaryEditor";

//actions
const char UNDO[]                  = "QDcmViewer.Undo";
const char REDO[]                  = "QDcmViewer.Redo";
const char COPY[]                  = "QDcmViewer.Copy";
const char PASTE[]                 = "QDcmViewer.Paste";
const char CUT[]                   = "QDcmViewer.Cut";
const char SELECTALL[]             = "QDcmViewer.SelectAll";

const char GOTO[]                  = "QDcmViewer.Goto";

const char NEW[]                   = "QDcmViewer.New";
const char OPEN[]                  = "QDcmViewer.Open";
const char OPEN_WITH[]             = "QDcmViewer.OpenWith";
const char REVERTTOSAVED[]         = "QDcmViewer.RevertToSaved";
const char SAVE[]                  = "QDcmViewer.Save";
const char SAVEAS[]                = "QDcmViewer.SaveAs";
const char SAVEALL[]               = "QDcmViewer.SaveAll";
const char PRINT[]                 = "QDcmViewer.Print";

const char OPTIONS[]               = "QDcmViewer.Options";
const char TOGGLE_LEFT_SIDEBAR[]   = "QDcmViewer.ToggleLeftSidebar";
const char TOGGLE_RIGHT_SIDEBAR[]  = "QDcmViewer.ToggleRightSidebar";
const char TOGGLE_MODE_SELECTOR[]  = "QDcmViewer.ToggleModeSelector";
const char TOGGLE_FULLSCREEN[]     = "QDcmViewer.ToggleFullScreen";
const char THEMEOPTIONS[]          = "QDcmViewer.ThemeOptions";

const char TR_SHOW_LEFT_SIDEBAR[]  = QT_TRANSLATE_NOOP("Core", "Show Left Sidebar");
const char TR_HIDE_LEFT_SIDEBAR[]  = QT_TRANSLATE_NOOP("Core", "Hide Left Sidebar");

const char TR_SHOW_RIGHT_SIDEBAR[] = QT_TRANSLATE_NOOP("Core", "Show Right Sidebar");
const char TR_HIDE_RIGHT_SIDEBAR[] = QT_TRANSLATE_NOOP("Core", "Hide Right Sidebar");

const char MINIMIZE_WINDOW[]       = "QDcmViewer.MinimizeWindow";
const char ZOOM_WINDOW[]           = "QDcmViewer.ZoomWindow";
const char CLOSE_WINDOW[]          = "QDcmViewer.CloseWindow";

const char SPLIT[]                 = "QDcmViewer.Split";
const char SPLIT_SIDE_BY_SIDE[]    = "QDcmViewer.SplitSideBySide";
const char SPLIT_NEW_WINDOW[]      = "QDcmViewer.SplitNewWindow";
const char REMOVE_CURRENT_SPLIT[]  = "QDcmViewer.RemoveCurrentSplit";
const char REMOVE_ALL_SPLITS[]     = "QDcmViewer.RemoveAllSplits";
const char GOTO_PREV_SPLIT[]       = "QDcmViewer.GoToPreviousSplit";
const char GOTO_NEXT_SPLIT[]       = "QDcmViewer.GoToNextSplit";
const char CLOSE[]                 = "QDcmViewer.Close";
const char CLOSE_ALTERNATIVE[]     = "QDcmViewer.Close_Alternative"; // temporary, see QRADIANTBUG-72
const char CLOSEALL[]              = "QDcmViewer.CloseAll";
const char CLOSEOTHERS[]           = "QDcmViewer.CloseOthers";
const char CLOSEALLEXCEPTVISIBLE[] = "QDcmViewer.CloseAllExceptVisible";
const char GOTONEXT[]              = "QDcmViewer.GotoNext";
const char GOTOPREV[]              = "QDcmViewer.GotoPrevious";
const char GOTONEXTINHISTORY[]     = "QDcmViewer.GotoNextInHistory";
const char GOTOPREVINHISTORY[]     = "QDcmViewer.GotoPreviousInHistory";
const char GO_BACK[]               = "QDcmViewer.GoBack";
const char GO_FORWARD[]            = "QDcmViewer.GoForward";
const char ABOUT_QRADIANT[]        = "QDcmViewer.AboutQRadiant";
const char ABOUT_PLUGINS[]         = "QDcmViewer.AboutPlugins";
const char S_RETURNTOEDITOR[]      = "QDcmViewer.ReturnToEditor";

// Default groups
const char G_DEFAULT_ONE[]         = "QDcmViewer.Group.Default.One";
const char G_DEFAULT_TWO[]         = "QDcmViewer.Group.Default.Two";
const char G_DEFAULT_THREE[]       = "QDcmViewer.Group.Default.Three";

// Main menu bar groups
const char G_FILE[]                = "QDcmViewer.Group.File";
const char G_EDIT[]                = "QDcmViewer.Group.Edit";
const char G_VIEW[]                = "QDcmViewer.Group.View";
const char G_WINDOW[]              = "QDcmViewer.Group.Window";

const char G_MENU_GROUP_BASE[]     = "QDcmViewer.Group.";
const char G_ARCHIVE[]             = "QDcmViewer.Group.Archive";      // ?????????????????????
const char G_FOLDER[]              = "QDcmViewer.Group.Folder";       // ????????????/?????????
const char G_DOWNLOAD[]            = "QDcmViewer.Group.Download";     // ?????????PACS??????
const char G_TOOLS[]               = "QDcmViewer.Group.Tools";        // ?????????
const char G_SYNCHRONIZE[]         = "QDcmViewer.Group.Synchronize";  // ????????????
const char G_SERIES_BROWSER[]      = "QDcmViewer.Group.SeriesBrowser"; //????????????
const char G_WL[]                  = "QDcmViewer.Group.WL";               // ??????????????????
const char G_PAN_IMAGE[]           = "QDcmViewer.Group.PanImage";         //????????????
const char G_ZOOM_IMAGE[]          = "QDcmViewer.Group.ZoomImage";        // ????????????
const char G_MEASURE[]             = "QDcmViewer.Group.Measure";          // ??????
const char G_ROTATE[]              = "QDcmViewer.Group.Rotate";           // ????????????
const char G_CINE_PLAY[]           = "QDcmViewer.Group.Cine";             // ????????????
const char G_MULTIPLANAR[]         = "QDcmViewer.Group.Multiplanar";      // ???????????????
const char G_FUSION[]              = "QDcmViewer.Group.Fusion";           // ????????????
const char G_HELP[]                = "QDcmViewer.Group.Help";

const char IMPORT[]                = "QDcmViewer.Import";
const char EXIT[]                  = "QDcmViewer.Exit";

const char HELP_ONLINE[]           = "QDcmViewer.Online";
const char HELP_CHLANGUAGE[]       = "QDcmViewer.ChangeLanguage";
const char HELP_CUSTOMIZE_KEY[]    = "QDcmViewer.CustomizeKey";
const char HELP_ABOUT_QDV[]        = "QDcmViewer.AboutQDV";
const char HELP_ABOUT_PLUGINS[]    = "QDcmViewer.AboutPlugins";
const char HELP_LANGUAGE_EN[]      = "QDcmViewer.LanguageEN";
const char HELP_LANGUAGE_CH[]      = "QDcmViewer.LanguageCH";
const char HELP_LANGUAGE_DOWNLOAD[]= "QDcmViewer.LanguageDownload";
const char HELP_LANGUAGE_LOAD[]    = "QDcmViewer.LanguageLoad";

const char HELP_FOLDER_OPEN_FOLDER[]= "QDcmViewer.OpenFolder";
const char HELP_FOLDER_OPEN_FILE[]  = "QDcmViewer.OpenFile";
const char HELP_FOLDER_OPEN_ZIPFILE[]  = "QDcmViewer.OpenZipFile";
const char HELP_FOLDER_OPEN_PKGFILE[]  = "QDcmViewer.OpenPKGFile";
const char EXPORT_TO_IMAGE[]           = "QDcmViewer.ExportToImage";
const char EXPORT_TO_CLIPBOARD[]       = "QDcmViewer.ExportToCpClipboard";
const char TOOLS_SPLIT[]               = "QDcmViewer.ToolsSplit";
const char TOOLS_SHOWTAG[]             = "QDcmViewer.ToolsShowTag";
const char TOOLS_SHOWANNO[]            = "QDcmViewer.ShowAnnotation";
const char TOOLS_SHOWCROSSLINE[]       = "QDcmViewer.ShowCrossLine";
const char TOOLS_HIDEPATIENT[]         = "QDcmViewer.HidePatientInfo";
const char TOOLS_HIDEMEASUREMENT[]     = "QDcmViewer.HideMeasurement";

// ????????????????????? ??????
const char G_ARCHIVE_IMPORT[]      = "QDcmViewer.Group.Archive.Import";
const char G_ARCHIVE_EXIT[]        = "QDcmViewer.Group.Archive.Exit";

// ????????????/????????? ??????
const char G_FOLDER_NEW[]          = "QDcmViewer.Group.Folder.New";
const char G_FOLDER_COPY[]         = "QDcmViewer.Group.Folder.Copy";
const char G_FOLDER_OPENFOLDER[]   = "QDcmViewer.Group.Folder.OpenFoler";
const char G_FOLDER_OPENFILE[]     = "QDcmViewer.Group.Folder.OpenFile";
const char G_FOLDER_ZIP[]          = "QDcmViewer.Group.Folder.Zip";
const char G_FOLDER_PKG[]          = "QDcmViewer.Group.Folder.PKG";

// ?????????PACS?????? ??????
const char G_DOWNLOAD_FIND[]       = "QDcmViewer.Group.Download.Find";
const char G_DOWNLOAD_ACCEPT[]     = "QDcmViewer.Group.Download.Accept";

// ?????? ??????
const char G_TOOLS_IMAGE[]             = "QDcmViewer.Group.Export.Image";
const char G_TOOLS_CPCLIPBOARD[]       = "QDcmViewer.Group.Export.CpClipboard";
const char G_TOOLS_CPALLCLIPBOARD[]    = "QDcmViewer.Group.Export.CyAllClipboard";
const char G_TOOLS_OPEN_MULSERIES[] = "QDcmViewer.Group.Split.MulSeries";          // ??????????????????
const char G_TOOLS_SPLIT_SERIES[]   = "QDcmViewer.Group.Split.Split";              // ??????
const char G_TOOLS_CLOSECURPANEL[]  = "QDcmViewer.Group.Split.CloseCurPanel";      // ??????????????????
const char G_TOOLS_CLOSEALLPANEL[]  = "QDcmViewer.Group.Split.CloseAllPanel";      // ??????????????????
const char G_TOOLS_SHOWTAG[]        = "QDcmViewer.Group.Split.ShowTags";           // ??????tag??????
const char G_TOOLS_ANNOTATION[]         = "QDcmViewer.Group.Annotation.Annotation";
const char G_TOOLS_CROSSLINE[]          = "QDcmViewer.Group.Annotation.CrossLine";   // ?????????
const char G_TOOLS_HIDEPATIENT[]        = "QDcmViewer.Group.Annotation.HidePatient";   // ??????????????????
const char G_TOOLS_HIDEMEASUREMENT[]    = "QDcmViewer.Group.Annotation.HideMeasurement";   // ??????????????????

// ????????????
const char G_SYNCHRONIZE_SWITCH[]         = "QDcmViewer.Group.Synchronize.Switch";    // ??????????????????
const char G_SYNCHRONIZE_SWITCH_SLICE[]   = "QDcmViewer.Group.Synchronize.SwitchSlice";    // ??????????????????

// ?????????????????? ??????
const char G_WL_DEFAULT[]                  = "QDcmViewer.Group.WL.Default";
const char G_WL_FULL_DYNAMIC[]             = "QDcmViewer.Group.WL.FullDynamic";
const char G_WL_FULL_CT_ABDOMEN[]          = "QDcmViewer.Group.WL.CTAbdomen";
const char G_WL_FULL_CT_ANGIO[]            = "QDcmViewer.Group.WL.CTAngio";
const char G_WL_FULL_CT_BONE[]             = "QDcmViewer.Group.WL.CTBone";
const char G_WL_FULL_CT_BRAIN[]            = "QDcmViewer.Group.WL.CTBrain";
const char G_WL_FULL_CT_CHEST[]            = "QDcmViewer.Group.WL.CTChest";
const char G_WL_FULL_CT_LUNGS[]            = "QDcmViewer.Group.WL.CTLungs";
const char G_WL_FULL_CT_NEGATIVE[]         = "QDcmViewer.Group.WL.Negative";      // ??????

 // ?????? ??????
const char G_MEASURE_LENGTH[]             = "QDcmViewer.Group.Measure.Length";    // ??????
const char G_MEASURE_ELIPSE[]             = "QDcmViewer.Group.Measure.Elipse";    // ????????????
const char G_MEASURE_CLOSED_POLYGON[]     = "QDcmViewer.Group.Measure.ClosedPolygon";    // ????????????
const char G_MEASURE_OPEN_POLYGON[]       = "QDcmViewer.Group.Measure.OpenPolygon";    //
const char G_MEASURE_ANGLE[]              = "QDcmViewer.Group.Measure.Angle";    // ??????
const char G_MEASURE_COBBANGLE[]          = "QDcmViewer.Group.Measure.CobbAngle";    // ???????????????
const char G_MEASURE_DEVIATION[]          = "QDcmViewer.Group.Measure.Deviation";    // ????????????
const char G_MEASURE_ARROW[]              = "QDcmViewer.Group.Measure.Arrow";    // ??????
const char G_MEASURE_PENCIL[]             = "QDcmViewer.Group.Measure.Pencil";    // ????????????
const char G_MEASURE_3DCURSOR[]           = "QDcmViewer.Group.Measure.3DCursor";    // 3D??????
const char G_MEASURE_EDITMEASUREMENTS[]   = "QDcmViewer.Group.Measure.EditMeasure";    // ????????????
const char G_MEASURE_ADVANCEDTOOLS[]      = "QDcmViewer.Group.Measure.AdvancedTools";    // ????????????

// ??????
const char G_ROTATE_CCW[]                 = "QDcmViewer.Group.Rotate.Ccw";                // ?????????90
const char G_ROTATE_CW[]                  = "QDcmViewer.Group.Rotate.Cw";                // ?????????90
const char G_ROTATE_180[]                 = "QDcmViewer.Group.Rotate.180";                // 180
const char G_ROTATE_HFLIP[]               = "QDcmViewer.Group.Rotate.FlipH";               // ????????????
const char G_ROTATE_VFLIP[]               = "QDcmViewer.Group.Rotate.FlipV";               // ????????????
const char G_ROTATE_CLEAR[]               = "QDcmViewer.Group.Rotate.Clear";               // ??????????????????

// ????????????
const char G_HELP_ONLINE[]                  = "QDcmViewer.Group.Help.Online";       // ????????????
const char G_HELP_CHANGE_LANGUAGE[]         = "QDcmViewer.Group.Help.ChangeLanguage";   // ????????????
const char G_HELP_CHANGE_CUSTOMIZE_KEY[]    = "QDcmViewer.Group.Help.CustomizeKey";   // ??????????????????
const char G_HELP_QRADIANT_ONLINE[]         = "QDcmViewer.Group.Help.QRadiantOnline";   //
const char G_HELP_ABOUT_QDV[]               = "QDcmViewer.Group.Help.AboutQDicomViewer";   //
const char G_HELP_ABOUT_PLUGIN[]            = "QDcmViewer.Group.Help.AboutPlugin";

// ????????????
const char G_HELP_LANGUAGE_EN[]         = "QDcmViewer.Group.Help.EN";   // ??????
const char G_HELP_LANGUAGE_CH[]         = "QDcmViewer.Group.Help.CH";   // ??????
const char G_HELP_LANGUAGE_SEPARATOR[]  = "QDcmViewer.Group.Help.Separator";   //
const char G_HELP_LANGUAGE_DOWNLOAD[]   = "QDcmViewer.Group.Help.Download";   // ????????????
const char G_HELP_LANGUAGE_LOAD[]       = "QDcmViewer.Group.Help.Download";   // ??????????????????

// File menu groups
const char G_FILE_NEW[]            = "QDcmViewer.Group.File.New";
const char G_FILE_OPEN[]           = "QDcmViewer.Group.File.Open";
const char G_FILE_PROJECT[]        = "QDcmViewer.Group.File.Project";
const char G_FILE_SAVE[]           = "QDcmViewer.Group.File.Save";
const char G_FILE_CLOSE[]          = "QDcmViewer.Group.File.Close";
const char G_FILE_PRINT[]          = "QDcmViewer.Group.File.Print";
const char G_FILE_OTHER[]          = "QDcmViewer.Group.File.Other";

// Edit menu groups
const char G_EDIT_UNDOREDO[]       = "QDcmViewer.Group.Edit.UndoRedo";
const char G_EDIT_COPYPASTE[]      = "QDcmViewer.Group.Edit.CopyPaste";
const char G_EDIT_SELECTALL[]      = "QDcmViewer.Group.Edit.SelectAll";
const char G_EDIT_ADVANCED[]       = "QDcmViewer.Group.Edit.Advanced";

const char G_EDIT_FIND[]           = "QDcmViewer.Group.Edit.Find";
const char G_EDIT_OTHER[]          = "QDcmViewer.Group.Edit.Other";

// Advanced edit menu groups
const char G_EDIT_FORMAT[]         = "QDcmViewer.Group.Edit.Format";
const char G_EDIT_COLLAPSING[]     = "QDcmViewer.Group.Edit.Collapsing";
const char G_EDIT_TEXT[]           = "QDcmViewer.Group.Edit.Text";
const char G_EDIT_BLOCKS[]         = "QDcmViewer.Group.Edit.Blocks";
const char G_EDIT_FONT[]           = "QDcmViewer.Group.Edit.Font";
const char G_EDIT_EDITOR[]         = "QDcmViewer.Group.Edit.Editor";

const char G_TOOLS_OPTIONS[]       = "QDcmViewer.Group.Tools.Options";

// Window menu groups
const char G_WINDOW_SIZE[]         = "QDcmViewer.Group.Window.Size";
const char G_WINDOW_PANES[]        = "QDcmViewer.Group.Window.Panes";
const char G_WINDOW_VIEWS[]        = "QDcmViewer.Group.Window.Views";
const char G_WINDOW_SPLIT[]        = "QDcmViewer.Group.Window.Split";
const char G_WINDOW_NAVIGATE[]     = "QDcmViewer.Group.Window.Navigate";
const char G_WINDOW_LIST[]         = "QDcmViewer.Group.Window.List";
const char G_WINDOW_OTHER[]        = "QDcmViewer.Group.Window.Other";

// Help groups (global)
const char G_HELP_HELP[]           = "QDcmViewer.Group.Help.Help";
const char G_HELP_LANGUAGE[]       = "QDcmViewer.Group.Help.Language";
const char G_HELP_SUPPORT[]        = "QDcmViewer.Group.Help.Supprt";
const char G_HELP_ABOUT[]          = "QDcmViewer.Group.Help.About";
const char G_HELP_UPDATES[]        = "QDcmViewer.Group.Help.Updates";

const char WIZARD_CATEGORY_QT[] = "R.Qt";
const char WIZARD_TR_CATEGORY_QT[] = QT_TRANSLATE_NOOP("Core", "Qt");
const char WIZARD_KIND_UNKNOWN[] = "unknown";
const char WIZARD_KIND_PROJECT[] = "project";
const char WIZARD_KIND_FILE[] = "file";

const char SETTINGS_CATEGORY_CORE[] = "A.Core";
const char SETTINGS_CATEGORY_CORE_ICON[] = ":/core/images/category_core.png";
const char SETTINGS_TR_CATEGORY_CORE[] = QT_TRANSLATE_NOOP("Core", "Environment");
const char SETTINGS_ID_INTERFACE[] = "A.Interface";
const char SETTINGS_ID_SYSTEM[] = "B.Core.System";
const char SETTINGS_ID_SHORTCUTS[] = "C.Keyboard";
const char SETTINGS_ID_TOOLS[] = "D.ExternalTools";
const char SETTINGS_ID_MIMETYPES[] = "E.MimeTypes";

const char SETTINGS_DEFAULTTEXTENCODING[] = "General/DefaultFileEncoding";

const char SETTINGS_THEME[] = "Core/CreatorTheme";
const char DEFAULT_THEME[] = "flat";

const char TR_CLEAR_MENU[]         = QT_TRANSLATE_NOOP("Core", "Clear Menu");

const char DEFAULT_BUILD_DIRECTORY[] = "../%{JS: Util.asciify(\"build-%{CurrentProject:Name}-%{CurrentKit:FileSystemName}-%{CurrentBuild:Name}\")}";

const int TARGET_ICON_SIZE = 32;

} // namespace Constants
} // namespace Core
