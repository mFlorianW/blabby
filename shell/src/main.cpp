/**
 ** This file is part of the Blabby project.
 ** Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as
 ** published by the Free Software Foundation, either version 2 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
#include "Clock.h"
#include "FileSystemPluginSource.h"
#include "MainController.h"
#include "MultiMediaPluginModel.h"
#include <QGuiApplication>
#include <QQuickView>
#include <qqml.h>

namespace
{
void registerQmlTypes()
{
    qmlRegisterType<Shell::MainController>("de.blabby.shell", 1, 0, "MainController");
    qmlRegisterType<Shell::MultiMediaPluginModel>("de.blabby.shell", 1, 0, "MultimediaPluginModel");
    qmlRegisterType<Shell::Clock>("de.blabby.shell", 1, 0, "Clock");
    qmlRegisterUncreatableType<Shell::MultiMediaPluginSource>("de.blabby.shell", 1, 0, "MultiMediaPluginSource", "");
    qmlRegisterType<Shell::FileSystemPluginSource>("de.blabby.shell", 1, 0, "FileSystemSource");
}

} // namespace

int main(int argc, char *argv[])
{
    QGuiApplication blabby(argc, argv);
    registerQmlTypes();

    QQuickView mainView;
    mainView.setMinimumSize(QSize{ 1280, 800 });
    mainView.setSource(QUrl("qrc:/shell/qml/MainWindow.qml"));

    mainView.show();

    return blabby.exec();
}
