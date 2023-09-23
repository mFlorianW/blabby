// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

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
    Shell::MainWindow mainWindow{&pluginModel, &fileSource};

    mainView.setMinimumSize(QSize{1280, 800});
    mainView.rootContext()->setContextProperty("g_mainWindow", &mainWindow);
    mainView.setSource(QUrl("qrc:/shell/qml/MainWindow.qml"));

    mainView.show();

    return blabby.exec();
}
