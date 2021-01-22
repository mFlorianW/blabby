/**
 ** This file is part of the Blabby project.
 ** Copyright 2021 Florian Weßel <florianwessel@gmx.net>.
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
