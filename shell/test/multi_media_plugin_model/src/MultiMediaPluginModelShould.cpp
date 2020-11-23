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

namespace Shell
{

class TestPlugin : public PluginCore::MultiMediaPlugin
{
    Q_DISABLE_COPY_MOVE(TestPlugin);

public:
    TestPlugin(){};

    QString pluginName() const override
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

    QUrl mainQMLUrl() const override
    {
        return QUrl{ "qrc:/qml/main.qml" };
    }

    QUrl pluginIconUrl() const override
    {
        return QUrl{ "qrc:/icon/pluginIcon.png" };
    }
};

MultiMediaPluginModelShould::MultiMediaPluginModelShould()
    : QObject()
{
}

void MultiMediaPluginModelShould::give_the_correct_role_names()
{
    QHash<qint32, QByteArray> expectedResult{
        std::make_pair(Shell::MultiMediaPluginModel::PluginName, "pluginName"),
        std::make_pair(Shell::MultiMediaPluginModel::PluginQmlUrl, "qmlUrl"),
        std::make_pair(Shell::MultiMediaPluginModel::PluginActive, "pluginActive"),
        std::make_pair(Shell::MultiMediaPluginModel::PluginIcoUrl, "pluginIconUrl")
    };

    Shell::MultiMediaPluginModel mediaPluginModel{};

    auto result = mediaPluginModel.roleNames();

    QCOMPARE(result, expectedResult);
}

void MultiMediaPluginModelShould::give_the_number_of_loaded_multimedia_plugins()
{
    TestPlugin testPlugin;
    Shell::MultiMediaPluginModel mediaPluginModel{ { std::make_shared<TestPlugin>() } };
    qint32 numberOfLoadedPluginsInSource = 1;

    qint32 numberLoadedPlugins = mediaPluginModel.rowCount({});

    QCOMPARE(numberLoadedPlugins, numberOfLoadedPluginsInSource);
}

void MultiMediaPluginModelShould::give_the_plugin_the_name()
{
    TestPlugin testPlugin;
    Shell::MultiMediaPluginModel mediaPluginModel{};
    mediaPluginModel.setPlugins({ std::make_shared<TestPlugin>() });

    auto pluginName = mediaPluginModel.data(mediaPluginModel.index(0), Shell::MultiMediaPluginModel::PluginName);

    QCOMPARE(pluginName, QVariant{ QStringLiteral("TestPlugin") });
}

void MultiMediaPluginModelShould::give_empty_variant_for_invalid_index()
{
    Shell::MultiMediaPluginModel mediaPluginModel{};
    mediaPluginModel.setPlugins({ std::make_shared<TestPlugin>() });

    auto pluginName = mediaPluginModel.data(mediaPluginModel.index(-1), Shell::MultiMediaPluginModel::PluginName);

    QCOMPARE(pluginName, QVariant{});
}

void MultiMediaPluginModelShould::give_qml_url_for_valid_index()
{
    TestPlugin testPlugin;
    Shell::MultiMediaPluginModel mediaPluginModel{ { std::make_shared<TestPlugin>() } };

    auto qmlUrl = mediaPluginModel.data(mediaPluginModel.index(0), Shell::MultiMediaPluginModel::PluginQmlUrl);

    QCOMPARE(qmlUrl, "qrc:/qml/main.qml");
}

void MultiMediaPluginModelShould::give_status_if_a_plugin_is_active()
{
    Shell::MultiMediaPluginModel mediaPluginModel{ { std::make_shared<TestPlugin>() } };
    QSignalSpy dataChangedSpy(&mediaPluginModel, &QAbstractItemModel::dataChanged);

    mediaPluginModel.setActivePlugin(0);
    auto activePlugin = mediaPluginModel.data(mediaPluginModel.index(0), Shell::MultiMediaPluginModel::PluginActive);

    QVERIFY(dataChangedSpy.size() == 1);
    QCOMPARE(activePlugin, true);
}

void MultiMediaPluginModelShould::give_a_icon_url_for_a_plugin()
{
    Shell::MultiMediaPluginModel mediaPluginModel{ { std::make_shared<TestPlugin>() } };

    auto pluginIconUrl = mediaPluginModel.data(mediaPluginModel.index(0), Shell::MultiMediaPluginModel::PluginIcoUrl);

    QCOMPARE(pluginIconUrl, "qrc:/icon/pluginIcon.png");
}

} // namespace Shell

QTEST_MAIN(Shell::MultiMediaPluginModelShould)
