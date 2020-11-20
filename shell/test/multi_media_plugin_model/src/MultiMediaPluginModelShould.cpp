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
#include "MultiMediaPluginModelShould.h"
#include "MultiMediaPlugin.h"
#include "MultiMediaPluginModel.h"
#include <QtTest>
#include <memory>

class TestPlugin : public PluginCore::MultiMediaPlugin
{
    Q_DISABLE_COPY_MOVE(TestPlugin);

public:
    TestPlugin(){};

    QString getPluginName() const override
    {
        return "TestPlugin";
    }

    PluginCore::PluginVersion getPluginVersion() const override
    {
        return { .major = 1, .minor = 0, .patch = 0 };
    }

    QUuid getPluginIdentifier() const override
    {
        return { "d6d02a93-b6f0-4581-ba0a-b70397c1200a" };
    }

    bool load() override
    {
        return true;
    }

    bool unload() override
    {
        return true;
    }

    QUrl getMainQML() const override
    {
        return QUrl{ "qrc:/qml/main.qml" };
    }
};

MultiMediaPluginModelShould::MultiMediaPluginModelShould()
    : QObject()
{
}

void MultiMediaPluginModelShould::give_the_number_of_loaded_multimedia_plugins()
{
    TestPlugin testPlugin;
    Shell::MultiMediaPluginModel mediaPluginModel{ { &testPlugin } };
    qint32 numberOfLoadedPluginsInSource = 1;

    qint32 numberLoadedPlugins = mediaPluginModel.rowCount({});

    QCOMPARE(numberLoadedPlugins, numberOfLoadedPluginsInSource);
}

void MultiMediaPluginModelShould::give_the_plugin_the_name()
{
    TestPlugin testPlugin;
    Shell::MultiMediaPluginModel mediaPluginModel{};
    mediaPluginModel.setPlugins({ &testPlugin });

    auto pluginName = mediaPluginModel.data(mediaPluginModel.index(0), Shell::MultiMediaPluginModel::PluginName);

    QCOMPARE(pluginName, QVariant{ QStringLiteral("TestPlugin") });
}

void MultiMediaPluginModelShould::give_empty_variant_for_invalid_index()
{
    TestPlugin testPlugin;
    Shell::MultiMediaPluginModel mediaPluginModel{ { &testPlugin } };

    auto pluginName = mediaPluginModel.data(mediaPluginModel.index(-1), Shell::MultiMediaPluginModel::PluginName);

    QCOMPARE(pluginName, QVariant{});
}

QTEST_MAIN(MultiMediaPluginModelShould)
