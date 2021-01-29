// Copyright 2021 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "MainWindowShould.h"
#include "MainWindow.h"
#include "MemoryPluginSource.h"
#include "MultiMediaPluginModel.h"
#include <QTest>

namespace Shell
{

MainWindowShould::MainWindowShould(QObject *parent)
    : QObject(parent)
{
}

void MainWindowShould::forward_back_button_call_to_plugin_when_plugin_active()
{
    MemoryPluginSource source;
    MultiMediaPluginModel model;
    MainWindow window{ &model, &source };
    window.activatePlugin(0);

    window.handleBackButtonPressed();

    QCOMPARE(source.plugin->handleBackButtonCalled, true);
}

} // namespace Shell

QTEST_MAIN(Shell::MainWindowShould)
