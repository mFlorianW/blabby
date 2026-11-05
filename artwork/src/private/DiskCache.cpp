// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "DiskCache.hpp"
#include "LoggingCategories.hpp"
#include <QCryptographicHash>
#include <QDir>
#include <QPromise>
#include <QSaveFile>
#include <QThreadPool>

namespace Artwork
{

namespace
{

constexpr auto jpegQuality = 90;

} // namespace

DiskCache::DiskCache(QString directory)
    : mDirectory{std::move(directory)}
{
}

QFuture<QImage> DiskCache::read(QString const& key) const
{
    auto promise = std::make_shared<QPromise<QImage>>();
    auto future = promise->future();
    QThreadPool::globalInstance()->start([promise, path = filePath(key)] {
        promise->start();
        auto image = QImage{};
        if (QFile::exists(path) and not image.load(path)) {
            // The album is looked up again and its Artwork written anew.
            qCWarning(artwork) << "Failed to read the cached Artwork" << path;
        }
        promise->addResult(image);
        promise->finish();
    });
    return future;
}

bool DiskCache::write(QString const& key, QImage const& image) const
{
    if (not QDir{}.mkpath(mDirectory)) {
        qCWarning(artwork) << "Failed to create the Artwork cache" << mDirectory;
        return false;
    }

    // Written to a temporary file first, so that a power loss leaves no broken image behind.
    auto file = QSaveFile{filePath(key)};
    if (not file.open(QIODevice::WriteOnly) or not image.save(&file, "JPG", jpegQuality) or not file.commit()) {
        qCWarning(artwork) << "Failed to write the Artwork to the cache" << file.fileName() << file.errorString();
        return false;
    }
    return true;
}

QString DiskCache::filePath(QString const& key) const
{
    auto const hash = QCryptographicHash::hash(key.toUtf8(), QCryptographicHash::Sha1).toHex();
    return QDir{mDirectory}.filePath(QString::fromLatin1(hash) + QStringLiteral(".jpg"));
}

} // namespace Artwork
