// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QGuiApplication>
#include <QQmlContext>
#include <QQuickView>

int main(int argc, char *argv[])
{
    QGuiApplication blabby(argc, argv);

    QQuickView mainView;

    mainView.setMinimumSize(QSize{1280, 800});
    mainView.setSource(QUrl("qrc:/shell/qml/MainWindow.qml"));

    mainView.show();

    return blabby.exec();
}
