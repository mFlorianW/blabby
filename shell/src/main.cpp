// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "ArtworkImageProvider.hpp"
#include "QmlSingletonRegistry.hpp"
#include <QGuiApplication>
#include <QIcon>
#include <QLoggingCategory>
#include <QQmlEngine>
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
}

} // namespace Shell

int main(int argc, char* argv[])
{
    QLoggingCategory::setFilterRules("*.debug=false");
    QGuiApplication blabby(argc, argv);
    // Shown in the title bar and the task bar when the shell runs on a desktop.
    QGuiApplication::setWindowIcon(QIcon{QStringLiteral(":/blabby/icons/blabby.svg")});

    Shell::registerQmlTypes();

    // Declared before the view, the image provider of the view's engine uses it.
    Artwork::OnlineLookup artworkLookup;

    QQuickView mainView;
    // The engine takes the ownership of the image provider.
    // NOLINTNEXTLINE cppcoreguidelines-owning-memory
    mainView.engine()->addImageProvider(QStringLiteral("artwork"), new Shell::ArtworkImageProvider{artworkLookup});
    mainView.setResizeMode(QQuickView::SizeRootObjectToView);
    mainView.setMinimumSize(QSize{800, 480});
    mainView.resize(QSize{1280, 720});
    mainView.setSource(QUrl("qrc:/qt/qml/Blabby/Shell/MainWindow.qml"));
    mainView.show();

    return blabby.exec();
}
