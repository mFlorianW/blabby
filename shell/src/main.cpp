// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "QmlSingletonRegistry.hpp"
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
}

} // namespace Shell

int main(int argc, char *argv[])
{
    QLoggingCategory::setFilterRules("*.debug=false");
    QGuiApplication blabby(argc, argv);

    Shell::registerQmlTypes();

    QQuickView mainView;
    mainView.setMinimumSize(QSize{1280, 800});
    mainView.setSource(QUrl("qrc:/qt/qml/Blabby/Shell/MainWindow.qml"));
    mainView.show();

    return blabby.exec();
}
