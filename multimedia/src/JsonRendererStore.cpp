// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "JsonRendererStore.hpp"
#include "private/LoggingCategories.hpp"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSaveFile>

namespace Multimedia
{

namespace
{
constexpr auto renderersKey = QLatin1StringView{"renderers"};
constexpr auto udnKey = QLatin1StringView{"udn"};
constexpr auto nameKey = QLatin1StringView{"name"};
constexpr auto manufacturerKey = QLatin1StringView{"manufacturer"};
constexpr auto modelNameKey = QLatin1StringView{"modelName"};
constexpr auto addressKey = QLatin1StringView{"address"};
} // namespace

JsonRendererStore::JsonRendererStore(QString filePath)
    : mFilePath{std::move(filePath)}
{
}

JsonRendererStore::~JsonRendererStore() = default;

QList<RememberedRenderer> JsonRendererStore::load() const
{
    auto file = QFile{mFilePath};
    if (not file.exists()) {
        return {};
    }

    if (not file.open(QIODevice::ReadOnly)) {
        qCWarning(mmRenderer) << "Failed to read the remembered Renderers from" << mFilePath
                              << "Error:" << file.errorString();
        return {};
    }

    auto parseError = QJsonParseError{};
    auto const document = QJsonDocument::fromJson(file.readAll(), &parseError);
    if (parseError.error != QJsonParseError::NoError or not document.isObject()) {
        qCWarning(mmRenderer) << "Failed to parse the remembered Renderers from" << mFilePath
                              << "Error:" << parseError.errorString();
        return {};
    }

    auto renderers = QList<RememberedRenderer>{};
    auto const array = document.object().value(renderersKey).toArray();
    for (auto const& value : array) {
        auto const object = value.toObject();
        auto const identity = object.value(udnKey).toString();
        if (identity.isEmpty()) {
            continue;
        }
        renderers.append(RememberedRenderer{.identity = identity,
                                            .name = object.value(nameKey).toString(),
                                            .manufacturer = object.value(manufacturerKey).toString(),
                                            .modelName = object.value(modelNameKey).toString(),
                                            .address = object.value(addressKey).toString()});
    }
    return renderers;
}

void JsonRendererStore::save(QList<RememberedRenderer> const& renderers)
{
    auto array = QJsonArray{};
    for (auto const& renderer : renderers) {
        array.append(QJsonObject{
            {udnKey, renderer.identity},
            {nameKey, renderer.name},
            {manufacturerKey, renderer.manufacturer},
            {modelNameKey, renderer.modelName},
            {addressKey, renderer.address},
        });
    }

    QDir{}.mkpath(QFileInfo{mFilePath}.absolutePath());
    auto file = QSaveFile{mFilePath};
    // QSaveFile ignores the write after a failed open and commit reports the failure.
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument{QJsonObject{{renderersKey, array}}}.toJson());
    }
    if (not file.commit()) {
        qCWarning(mmRenderer) << "Failed to write the remembered Renderers to" << mFilePath
                              << "Error:" << file.errorString();
    }
}

} // namespace Multimedia
