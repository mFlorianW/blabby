// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "MultiMediaPluginModelShould.h"
#include "MultiMediaPluginModel.h"
#include "MultimediaPlugin.h"
#include "TestPlugin.h"
#include <QtTest>
#include <memory>

namespace Shell
{

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
        std::make_pair(Shell::MultiMediaPluginModel::PluginIcoUrl, "pluginIconUrl")};

    Shell::MultiMediaPluginModel mediaPluginModel{};

    auto result = mediaPluginModel.roleNames();

    QCOMPARE(result, expectedResult);
}

void MultiMediaPluginModelShould::give_the_number_of_loaded_multimedia_plugins()
{
    TestPlugin testPlugin;
    Shell::MultiMediaPluginModel mediaPluginModel{{std::make_shared<TestPlugin>()}};
    qint32 numberOfLoadedPluginsInSource = 1;

    qint32 numberLoadedPlugins = mediaPluginModel.rowCount({});

    QCOMPARE(numberLoadedPlugins, numberOfLoadedPluginsInSource);
}

void MultiMediaPluginModelShould::give_the_plugin_the_name()
{
    TestPlugin testPlugin;
    Shell::MultiMediaPluginModel mediaPluginModel{};
    mediaPluginModel.setPlugins({std::make_shared<TestPlugin>()});

    auto pluginName = mediaPluginModel.data(mediaPluginModel.index(0), Shell::MultiMediaPluginModel::PluginName);

    QCOMPARE(pluginName, QVariant{QStringLiteral("TestPlugin")});
}

void MultiMediaPluginModelShould::give_empty_variant_for_invalid_index()
{
    Shell::MultiMediaPluginModel mediaPluginModel{};
    mediaPluginModel.setPlugins({std::make_shared<TestPlugin>()});

    auto pluginName = mediaPluginModel.data(mediaPluginModel.index(-1), Shell::MultiMediaPluginModel::PluginName);

    QCOMPARE(pluginName, QVariant{});
}

void MultiMediaPluginModelShould::give_status_if_a_plugin_is_active()
{
    Shell::MultiMediaPluginModel mediaPluginModel{{std::make_shared<TestPlugin>()}};
    QSignalSpy dataChangedSpy(&mediaPluginModel, &QAbstractItemModel::dataChanged);

    mediaPluginModel.setActivePlugin(0);
    auto activePlugin = mediaPluginModel.data(mediaPluginModel.index(0), Shell::MultiMediaPluginModel::PluginActive);

    QVERIFY(dataChangedSpy.size() == 1);
    QCOMPARE(activePlugin, true);
}

void MultiMediaPluginModelShould::give_a_icon_url_for_a_plugin()
{
    Shell::MultiMediaPluginModel mediaPluginModel{{std::make_shared<TestPlugin>()}};

    auto pluginIconUrl =
        mediaPluginModel.data(mediaPluginModel.index(0), Shell::MultiMediaPluginModel::PluginIcoUrl).toString();

    QCOMPARE(pluginIconUrl, "qrc:/icon/pluginIcon.png");
}

void MultiMediaPluginModelShould::should_give_a_multimedia_plugin_object()
{
    auto object = std::make_shared<TestPlugin>();
    Shell::MultiMediaPluginModel mediaPluginModel{{object}};
    auto expectedObject = object.get();

    auto result = mediaPluginModel.plugin(0);

    QCOMPARE(result, expectedObject);
}

} // namespace Shell

QTEST_MAIN(Shell::MultiMediaPluginModelShould)
