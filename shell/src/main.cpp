// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "MediaPlayer.hpp"
#include "QmlSingletonRegistry.hpp"
#include "Renderer.hpp"
#include <QGuiApplication>
#include <QLoggingCategory>
#include <QQuickView>

namespace Shell
{

void registerQmlTypes()
{
    qmlRegisterSingletonType<QmlSingletonRegistry>("Blabby.Singleton",
                                                   1,
                                                   0,
                                                   "Singleton",
                                                   &QmlSingletonRegistry::createQmlRegistry);
    qmlRegisterUncreatableType<Shell::MediaPlayer>("Blabby.Objects", 1, 0, "MediaPlayer", "");
    qmlRegisterUncreatableType<Multimedia::Renderer>("Blabby.Objects",
                                                     1,
                                                     0,
                                                     "Renderer",
                                                     "Renderers are provided by the MediaRendererModel");
}

} // namespace Shell

int main(int argc, char* argv[])
{
    QLoggingCategory::setFilterRules("*.debug=false");
    QGuiApplication blabby(argc, argv);

    Shell::registerQmlTypes();

    QQuickView mainView;
    mainView.setResizeMode(QQuickView::SizeRootObjectToView);
    mainView.setMinimumSize(QSize{800, 480});
    mainView.resize(QSize{1280, 720});
    mainView.setSource(QUrl("qrc:/qt/qml/Blabby/Shell/MainWindow.qml"));
    mainView.show();

    return blabby.exec();
}
