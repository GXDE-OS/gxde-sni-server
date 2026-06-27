cmake_minimum_required(VERSION 3.16)

project(gxde-sni-server)
set(PROJECT_VERSION "6.7.1")
string(REPLACE "." ";" VERSION_LIST ${PROJECT_VERSION})
list(GET VERSION_LIST 0 PROJECT_VERSION_MAJOR)
list(GET VERSION_LIST 1 PROJECT_VERSION_MINOR)
list(GET VERSION_LIST 2 PROJECT_VERSION_PATCH)

set(QT_MIN_VERSION "6.10.0")
set(KF6_MIN_VERSION "6.26.0")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ECM ${KF6_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMMarkNonGuiExecutable)
include(FeatureSummary)
include(ECMQtDeclareLoggingCategory)
include(ECMInstallIcons)
include(ECMConfiguredInstall)

find_package(Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Core DBus Gui)

find_package(KF6 ${KF6_MIN_VERSION} REQUIRED COMPONENTS
    CoreAddons
    Crash
    DBusAddons
    StatusNotifierItem
    WindowSystem
)

# For xembed-sni-proxy
find_package(X11 REQUIRED)
find_package(XCB MODULE REQUIRED COMPONENTS XCB XFIXES DAMAGE COMPOSITE RANDR SHM UTIL IMAGE ICCCM)

set(HAVE_X11 1)
set(HAVE_XCURSOR 1)
set(HAVE_XFIXES 1)

include_directories("${CMAKE_CURRENT_BINARY_DIR}")

configure_file(config-workspace.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-workspace.h)

add_subdirectory(statusnotifierwatcher)
ecm_optional_add_subdirectory(xembed-sni-proxy)

ecm_qt_install_logging_categories(
    EXPORT GXDESNISERVER
    FILE gxde-sni-server.categories
    DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR}
)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
