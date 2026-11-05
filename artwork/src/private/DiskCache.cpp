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
#include <numeric>

namespace Artwork
{

namespace
{

constexpr auto jpegQuality = 90;

} // namespace

DiskCache::DiskCache(QString directory, qint64 capacity)
    : mDirectory{std::move(directory)}
    , mCapacity{capacity}
{
    auto const entries = QDir{mDirectory}.entryInfoList(QDir::Files, QDir::Time);
    if (not entries.isEmpty()) {
        mLastUse = entries.constFirst().lastModified();
    }
}

bool DiskCache::contains(QString const& key) const
{
    return QFile::exists(filePath(key));
}

QFuture<QImage> DiskCache::read(QString const& key)
{
    auto promise = std::make_shared<QPromise<QImage>>();
    auto future = promise->future();
    auto path = filePath(key);
    // Marked right away rather than in the reading thread, so that a write in between doesn't remove it.
    markUsed(path);
    QThreadPool::globalInstance()->start([promise, path = std::move(path)] {
        promise->start();
        auto image = QImage{};
        if (not image.load(path)) {
            qCWarning(artwork) << "Failed to read the cached Artwork" << path;
        }
        promise->addResult(image);
        promise->finish();
    });
    return future;
}

bool DiskCache::write(QString const& key, QImage const& image)
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
    markUsed(file.fileName());
    evict();
    return true;
}

QString DiskCache::filePath(QString const& key) const
{
    auto const hash = QCryptographicHash::hash(key.toUtf8(), QCryptographicHash::Sha1).toHex();
    return QDir{mDirectory}.filePath(QString::fromLatin1(hash) + QStringLiteral(".jpg"));
}

void DiskCache::markUsed(QString const& path)
{
    auto file = QFile{path};
    if (not file.open(QIODevice::ReadOnly) or not file.setFileTime(nextUse(), QFileDevice::FileModificationTime)) {
        qCWarning(artwork) << "Failed to mark the cached Artwork as used" << path << file.errorString();
    }
}

QDateTime DiskCache::nextUse()
{
    // Strictly increasing, so that uses in the same millisecond keep their order, and later than the uses before a
    // restart, also when the clock went back, e.g. on a device without a real-time clock before it's synchronised.
    auto use = QDateTime::currentDateTimeUtc();
    if (mLastUse.isValid() and use <= mLastUse) {
        use = mLastUse.addMSecs(1);
    }
    mLastUse = use;
    return use;
}

void DiskCache::evict() const
{
    auto entries = QDir{mDirectory}.entryInfoList(QDir::Files, QDir::Time);
    auto size = std::accumulate(entries.cbegin(), entries.cend(), qint64{0}, [](qint64 sum, QFileInfo const& entry) {
        return sum + entry.size();
    });
    // The entries are sorted from the most to the least recently used one.
    while (size > mCapacity and not entries.isEmpty()) {
        auto const entry = entries.takeLast();
        if (not QFile::remove(entry.filePath())) {
            qCWarning(artwork) << "Failed to remove the Artwork from the cache" << entry.filePath();
            continue;
        }
        size -= entry.size();
    }
}

} // namespace Artwork
