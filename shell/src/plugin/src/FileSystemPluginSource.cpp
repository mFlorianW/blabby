// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "FileSystemPluginSource.h"
#include "FileSystemPluginSource_p.h"
#include "LoggingCategories.h"
#include "MultimediaPlugin.h"
#include "PluginDirs.h"
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QLibrary>
#include <QPluginLoader>

namespace Shell
{

FileSystemPluginSourcePrivate::FileSystemPluginSourcePrivate() = default;
FileSystemPluginSourcePrivate::~FileSystemPluginSourcePrivate() = default;


void FileSystemPluginSourcePrivate::loadPlugins(const QFileInfo &fileInfo)
{
    QPluginLoader pluginLoader{ fileInfo.filePath() };

    if(!pluginLoader.load())
    {
        qCWarning(shell) << "Failed to load plugin fileName:" << fileInfo.fileName() << "Error:" << pluginLoader.errorString();
        //        m_failedToLoadPlugins.append(fileInfo.fileName());
        return;
    }

    // Load the plugin, if the library doesn't implements the MultiMediaPlugin
    // interface then we can ignore this file and it can be unloaded.
    auto plugin =
        std::shared_ptr<PluginCore::MultimediaPlugin>(qobject_cast<PluginCore::MultimediaPlugin *>(pluginLoader.instance()));

    if(plugin != nullptr)
    {
        plugin->load(mContext);
        mLoadedPlugins.append(std::move(plugin));
    }
    else
    {
        pluginLoader.unload();
        qCCritical(shell) << "Plugin:" << fileInfo.fileName() << "Error: Plugin is not a PluginCore::MultiMediaPlugin";
    }
}

FileSystemPluginSource::FileSystemPluginSource()
    : MultiMediaPluginSource()
    , d(std::make_unique<FileSystemPluginSourcePrivate>())
{
}

FileSystemPluginSource::~FileSystemPluginSource() = default;

void FileSystemPluginSource::loadPlugins() const
{
    for(const QString &path : { DEFAULT_MULTIMEDIA_PLUGIN_INSTALL_DIR, DEFAULT_QML_PLUGIN_DIR })
    {
        QDir dir{ path };
        if(!dir.exists(path))
        {
            qCWarning(shell) << "Plugin folder" << dir.path() << "doesn't exists. Skipping";
            continue;
        }

        qCDebug(shell) << "Searching: " << path;
        QDirIterator dirIter{ path, QDirIterator::Subdirectories };
        while(dirIter.hasNext())
        {
            QFileInfo fileInfo = dirIter.fileInfo();
            if(fileInfo.isFile() && QLibrary::isLibrary(fileInfo.fileName()))
            {
                d->loadPlugins(fileInfo);
            }

            qCDebug(shell) << dirIter.fileName();
            //            qCDebug(shell) << fileInfo.absoluteFilePath();
            // go to the next entry
            dirIter.next();
        }
    }
}

void Shell::FileSystemPluginSource::setPluginQQmlContext(QQmlContext *context) noexcept
{
    d->mContext = context;
}

QVector<std::shared_ptr<PluginCore::MultimediaPlugin>> FileSystemPluginSource::plugins() const
{
    return d->mLoadedPlugins;
}

} // namespace Shell
