/* Install paths from configure */

static const char QT_INSTALL_PREFIX      [267] = "qt_nstpath=/home/ro/Documents/qt3";
static const char QT_INSTALL_BINS        [267] = "qt_binpath=/home/ro/Documents/qt3/bin";
static const char QT_INSTALL_DOCS        [267] = "qt_docpath=/home/ro/Documents/qt3/doc";
static const char QT_INSTALL_HEADERS     [267] = "qt_hdrpath=/home/ro/Documents/qt3/include";
static const char QT_INSTALL_LIBS        [267] = "qt_libpath=/home/ro/Documents/qt3/lib";
static const char QT_INSTALL_PLUGINS     [267] = "qt_plgpath=/home/ro/Documents/qt3/plugins";
static const char QT_INSTALL_DATA        [267] = "qt_datpath=/home/ro/Documents/qt3";
static const char QT_INSTALL_TRANSLATIONS[267] = "qt_trnpath=/home/ro/Documents/qt3/translations";
static const char QT_INSTALL_SYSCONF     [267] = "qt_cnfpath=/home/ro/Documents/qt3/etc/settings";

/* strlen( "qt_xxxpath=" ) == 11 */
const char *qInstallPath()             { return QT_INSTALL_PREFIX       + 11; }
const char *qInstallPathDocs()         { return QT_INSTALL_DOCS         + 11; }
const char *qInstallPathHeaders()      { return QT_INSTALL_HEADERS      + 11; }
const char *qInstallPathLibs()         { return QT_INSTALL_LIBS         + 11; }
const char *qInstallPathBins()         { return QT_INSTALL_BINS         + 11; }
const char *qInstallPathPlugins()      { return QT_INSTALL_PLUGINS      + 11; }
const char *qInstallPathData()         { return QT_INSTALL_DATA         + 11; }
const char *qInstallPathTranslations() { return QT_INSTALL_TRANSLATIONS + 11; }
const char *qInstallPathSysconf()      { return QT_INSTALL_SYSCONF      + 11; }
