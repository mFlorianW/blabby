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
    TestableMultiMediaPlugin()
    {
    }

    QString pluginName() const override
    {
    }

    PluginVersion getPluginVersion() const override
    {
    }

    QUuid getPluginIdentifier() const override
    {
    }

    bool load(QQmlContext *conntext) override
    {
        Q_UNUSED(conntext)
    }

    bool unload() override
    {
    }

    QUrl mainQMLUrl() const override
    {
        return {};
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
    QSignalSpy activeViewSpy{ &pl, &TestableMultiMediaPlugin::activeViewChanged };
    QUrl expectedUrl{ "qrc:/main.qml" };

    pl.setActiveView(QUrl{ "qrc:/main.qml" });

    QVERIFY(activeViewSpy.size() == 1);
    QCOMPARE(pl.activeView(), expectedUrl);
}

} // namespace PluginCore


QTEST_MAIN(PluginCore::MultimediaPluginShould);
#include "MultimediaPluginShould.moc"
