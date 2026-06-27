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
include(FeatureSummary)
include(ECMQtDeclareLoggingCategory)
include(ECMConfiguredInstall)

find_package(Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Core DBus)

find_package(KF6 ${KF6_MIN_VERSION} REQUIRED COMPONENTS
    CoreAddons
    Crash
    DBusAddons
    KDED
    StatusNotifierItem
)

include_directories("${CMAKE_CURRENT_BINARY_DIR}")

configure_file(config-workspace.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-workspace.h)

set(SNI_SERVER_SRCS
    src/main.cpp
    src/statusnotifierwatcher/statusnotifierwatcher.cpp
    src/statusnotifierwatcher/statusnotifierwatcher.h
    src/statusnotifierwatcher/systemtraytypedefs.h
)

# DBus adaptor: StatusNotifierWatcher
qt_add_dbus_adaptor(SNI_SERVER_SRCS
    ${KSTATUSNOTIFIERITEM_DBUS_INTERFACES_DIR}/kf6_org.kde.StatusNotifierWatcher.xml
    src/statusnotifierwatcher/statusnotifierwatcher.h StatusNotifierWatcher
)

# DBus interface: StatusNotifierItem (used to validate registered items)
set(statusnotifieritem_xml ${KSTATUSNOTIFIERITEM_DBUS_INTERFACES_DIR}/kf6_org.kde.StatusNotifierItem.xml)
set_source_files_properties(${statusnotifieritem_xml} PROPERTIES
    NO_NAMESPACE false
    INCLUDE "systemtraytypedefs.h"
    CLASSNAME OrgKdeStatusNotifierItemInterface
)
qt_add_dbus_interface(SNI_SERVER_SRCS ${statusnotifieritem_xml} statusnotifieritem_interface)

ecm_qt_declare_logging_category(SNI_SERVER_SRCS
    HEADER statusnotifierwatcher_debug.h
    IDENTIFIER STATUSNOTIFIERITEMWATCHER
    CATEGORY_NAME org.kde.plasma.statusnotifierwatcher
    DESCRIPTION "Status Notifier Manager"
    EXPORT GXDESNISERVER
)

add_executable(gxde-sni-server ${SNI_SERVER_SRCS})

target_include_directories(gxde-sni-server PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/statusnotifierwatcher
)

target_link_libraries(gxde-sni-server
    Qt::Core
    Qt::DBus
    KF6::CoreAddons
    KF6::Crash
    KF6::DBusAddons
    KF6::KDED
    KF6::StatusNotifierItem
)

install(TARGETS gxde-sni-server ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

ecm_qt_install_logging_categories(
    EXPORT GXDESNISERVER
    FILE gxde-sni-server.categories
    DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR}
)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
