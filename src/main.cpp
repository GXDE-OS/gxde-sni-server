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

static bool isWayland() {
    return qgetenv("XDG_SESSION_TYPE") == QByteArrayLiteral("wayland");
}

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);

    KAboutData about(QStringLiteral("gxde-sni-server"),
        QStringLiteral("GXDE SNI Server"),
        QStringLiteral(WORKSPACE_VERSION_STRING));
    KAboutData::setApplicationData(about);
    KCrash::initialize();

    if (!isWayland()) {
        qInfo() << "(Init) Wayland-check: XDG_SESSION_TYPE is NOT wayland!!"
                << "dde-daemon handles SNI on X11, exiting normally...";
        return 0;
    }

    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    if (!sessionBus.registerService(QStringLiteral("org.kde.StatusNotifierWatcher"))) {
        qCritical() << "(DBus) Reg: Failed to register server D-Bus."
                     << "Is another instance already running?";
        return -1;
    }

    auto *watcher = new StatusNotifierWatcher(nullptr, {});
    KDBusService service(KDBusService::Unique);

    qInfo() << "(Main) Init: GXDE SNI Server shall be started.";
    return app.exec();
}
