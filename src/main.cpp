/**
 * Copyright (C) 2026 CharOfString <markus_verify@126.com>
 *
 * This file is part of gxde-sni-server.
 *
 * gxde-sni-server is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * gxde-sni-server is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gxde-sni-server.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <QCoreApplication>
#include <QDBusConnection>
#include <QDebug>

#include <KAboutData>
#include <KCrash>
#include <KDBusService>

#include "config-workspace.h"
#include "statusnotifierwatcher.h"

bool isWayland() {
    return qgetenv("XDG_SESSION_TYPE").compare("wayland") == 0;
}

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);

    KAboutData aboutData(QStringLiteral("gxde-sni-server"),
        QStringLiteral("GXDE SNI Server"),
        QStringLiteral(WORKSPACE_VERSION_STRING));

    KAboutData::setApplicationData(aboutData);
    KCrash::initialize();
    app.setQuitOnLastWindowClosed(false);

    if (!isWayland()) {
        qInfo()
            << "(Init) Wayland-check: GXDE SNI Server is designed for wayland."
            << "On X11 dde-daemon will handle the SNI registry,"
            << "exiting normally..."
        return 0;
    }

    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    if (!sessionBus.registerService(QStringLiteral("org.kde.StatusNotifierWatcher"))) {
        qFatal() << "(DBus) Reg: Failed to register"
            << "org.kde.StatusNotifierWatcher.";
        qFatal() << "            Is another instance running?";
        exit -1;
    }

    auto* watcher = new StatusNotifierWatcher(nullptr, {});
    KDBusService service(KDBusService::Unique);
    return app.exec();
}
