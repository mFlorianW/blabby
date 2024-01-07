// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ProviderLoader.hpp"
#include "private/LoggingCategories.hpp"
#include <QDir>
#include <QDirIterator>
#include <QPluginLoader>

namespace Multimedia
{

ProviderLoader::ProviderLoader() = default;
ProviderLoader::~ProviderLoader() = default;

Providers const &ProviderLoader::load(QStringList const &paths) noexcept
{
    for (QString const &path : std::as_const(paths)) {
        QDir dir{path};
        if (!dir.exists(path)) {
            qCWarning(mmProvider) << "Plugin folder" << dir.path() << "doesn't exists. Skipping";
            continue;
        }

        qCDebug(mmProvider) << "Searching: " << path;
        QDirIterator dirIter{path, QDirIterator::Subdirectories};
        while (dirIter.hasNext()) {
            dirIter.next();
            qCDebug(mmProvider) << dirIter.fileName();
            QFileInfo fileInfo = dirIter.fileInfo();
            if (fileInfo.isFile() && QLibrary::isLibrary(fileInfo.fileName())) {
                loadProvider(fileInfo);
            }
        }
    }
    return mProviders;
}
void ProviderLoader::loadProvider(QFileInfo const &provider) noexcept
{
    QPluginLoader pluginLoader{provider.filePath()};
    if (!pluginLoader.load()) {
        qCWarning(mmProvider) << "Failed to load plugin fileName:" << provider.fileName()
                              << "Error:" << pluginLoader.errorString();
        return;
    }

    // Load the plugin, if the library doesn't implements the Provider
    // interface then we can ignore this file and it can be unloaded.
    auto plugin = std::shared_ptr<Provider>(qobject_cast<Provider *>(pluginLoader.instance()));
    if (plugin != nullptr) {
        mProviders.append(std::move(plugin));
    } else {
        pluginLoader.unload();
        qCCritical(mmProvider) << "Plugin:" << provider.fileName() << "Error: Plugin is not a Multimedia::Provider";
    }
}

} // namespace Multimedia
