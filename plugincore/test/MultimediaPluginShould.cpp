// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MultimediaPluginShould.h"
#include "MultimediaPlugin.h"
#include <QSignalSpy>
#include <QUrl>
#include <QtTest>

namespace PluginCore
{

class TestableMultiMediaPlugin : public MultimediaPlugin
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(TestableMultiMediaPlugin)
public:
    TestableMultiMediaPlugin() = default;
    ~TestableMultiMediaPlugin() override = default;

    QString pluginName() const override
    {
        return "";
    }

    PluginVersion getPluginVersion() const override
    {
        return {.major = 0, .minor = 0, .patch = 0};
    }

    QUuid getPluginIdentifier() const override
    {
        return QUuid{};
    }

    bool load(QQmlContext *conntext) override
    {
        Q_UNUSED(conntext);
        return true;
    }

    bool unload() override
    {
        return true;
    }

    QUrl pluginIconUrl() const override
    {
        return {};
    }
};

MultimediaPluginShould::MultimediaPluginShould(QObject *parent)
    : QObject(parent)
{
}

void MultimediaPluginShould::be_able_to_set_a_active_view()
{
    TestableMultiMediaPlugin pl;
    QSignalSpy activeViewSpy{&pl, &TestableMultiMediaPlugin::activeViewChanged};
    QUrl expectedUrl{"qrc:/main.qml"};

    pl.setActiveView(QUrl{"qrc:/main.qml"});

    QVERIFY(activeViewSpy.size() == 1);
    QCOMPARE(pl.activeView(), expectedUrl);
}

} // namespace PluginCore

QTEST_MAIN(PluginCore::MultimediaPluginShould);
#include "MultimediaPluginShould.moc"
