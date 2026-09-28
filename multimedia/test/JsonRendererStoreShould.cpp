// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "JsonRendererStoreShould.hpp"
#include "JsonRendererStore.hpp"
#include <QFile>
#include <QTest>

namespace Multimedia
{

namespace
{
RememberedRenderer kitchen()
{
    return RememberedRenderer{.identity = QStringLiteral("uuid:kitchen"),
                              .name = QStringLiteral("Kitchen"),
                              .manufacturer = QStringLiteral("Denon"),
                              .modelName = QStringLiteral("HEOS 1"),
                              .address = QStringLiteral("192.168.1.42")};
}

RememberedRenderer bathroom()
{
    return RememberedRenderer{.identity = QStringLiteral("uuid:bathroom"),
                              .name = QStringLiteral("Bathroom"),
                              .manufacturer = {},
                              .modelName = {},
                              .address = {}};
}
} // namespace

JsonRendererStoreShould::~JsonRendererStoreShould() = default;

void JsonRendererStoreShould::init()
{
    mDir = std::make_unique<QTemporaryDir>();
    QVERIFY(mDir->isValid());
}

QString JsonRendererStoreShould::storePath() const
{
    return mDir->filePath(QStringLiteral("renderers.json"));
}

void JsonRendererStoreShould::give_the_saved_renderers_after_a_restart()
{
    JsonRendererStore{storePath()}.save({kitchen(), bathroom()});

    auto const renderers = JsonRendererStore{storePath()}.load();

    QCOMPARE(renderers, (QList<RememberedRenderer>{kitchen(), bathroom()}));
}

void JsonRendererStoreShould::replace_the_remembered_renderers_on_save()
{
    auto store = JsonRendererStore{storePath()};
    store.save({kitchen(), bathroom()});

    store.save({bathroom()});

    QCOMPARE(store.load(), QList<RememberedRenderer>{bathroom()});
}

void JsonRendererStoreShould::give_no_renderers_when_the_file_is_missing()
{
    auto const renderers = JsonRendererStore{storePath()}.load();

    QCOMPARE(renderers, QList<RememberedRenderer>{});
}

void JsonRendererStoreShould::give_no_renderers_when_the_file_is_corrupt()
{
    auto file = QFile{storePath()};
    QVERIFY(file.open(QIODevice::WriteOnly));
    file.write(R"({"renderers": [ {"udn": )");
    file.close();

    auto const renderers = JsonRendererStore{storePath()}.load();

    QCOMPARE(renderers, QList<RememberedRenderer>{});
}

void JsonRendererStoreShould::create_the_missing_directory_on_save()
{
    auto const path = mDir->filePath(QStringLiteral("blabby/renderers.json"));

    JsonRendererStore{path}.save({kitchen()});

    QCOMPARE(JsonRendererStore{path}.load(), QList<RememberedRenderer>{kitchen()});
}

} // namespace Multimedia

QTEST_MAIN(Multimedia::JsonRendererStoreShould)
