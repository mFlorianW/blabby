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
#include "MainWindow.h"
#include "MultiMediaPluginModel.h"
#include <QGuiApplication>
#include <QQmlContext>
#include <QQuickView>
#include <qqml.h>

namespace
{

void registerQmlTypes()
{
    qmlRegisterUncreatableType<Shell::MultiMediaPluginModel>("de.blabby.shell", 1, 0, "MultimediaPluginModel", "");
    qmlRegisterType<Shell::Clock>("de.blabby.shell", 1, 0, "Clock");
}

} // namespace

int main(int argc, char *argv[])
{
    QGuiApplication blabby(argc, argv);
    registerQmlTypes();

    QQuickView mainView;
    Shell::MultiMediaPluginModel pluginModel;
    Shell::FileSystemPluginSource fileSource;
    fileSource.setPluginQQmlContext(mainView.rootContext());
    Shell::MainWindow mainWindow{ &pluginModel, &fileSource };

    mainView.setMinimumSize(QSize{ 1280, 800 });
    mainView.rootContext()->setContextProperty("g_mainWindow", &mainWindow);
    mainView.setSource(QUrl("qrc:/shell/qml/MainWindow.qml"));

    mainView.show();

    return blabby.exec();
}
