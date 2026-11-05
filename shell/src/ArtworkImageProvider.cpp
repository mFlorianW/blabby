// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "ArtworkImageProvider.hpp"
#include <QUrlQuery>

namespace Shell
{

namespace
{

/**
 * The response of the @ref ArtworkImageProvider. QML requests and cancels it in its image loading thread, so it
 * lives in the thread of the lookup and talks to the lookup only there. The image and the error are written there
 * before the finished signal, QML reads them in its thread only after that signal.
 */
class ArtworkImageResponse final : public QQuickImageResponse
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(ArtworkImageResponse)
public:
    ArtworkImageResponse(Artwork::Lookup& lookup, QString artist, QString album, QSize const& requestedSize)
        : mRequestedSize{requestedSize}
    {
        moveToThread(lookup.thread());
        QMetaObject::invokeMethod(
            this,
            [this, &lookup, artist = std::move(artist), album = std::move(album)] {
                mRequest = lookup.requestArtwork(artist, album);
                connect(mRequest.get(), &Artwork::Request::finished, this, &ArtworkImageResponse::onRequestFinished);
            },
            Qt::QueuedConnection);
    }

    ~ArtworkImageResponse() override = default;

    QQuickTextureFactory* textureFactory() const override
    {
        if (mImage.isNull()) {
            return nullptr;
        }
        return QQuickTextureFactory::textureFactoryForImage(mImage);
    }

    QString errorString() const override
    {
        return mErrorString;
    }

    void cancel() override
    {
        QMetaObject::invokeMethod(
            this,
            [this] {
                if (mRequest != nullptr) {
                    mRequest->cancel();
                }
            },
            Qt::QueuedConnection);
    }

private:
    void onRequestFinished()
    {
        mImage = mRequest->image();
        if (mImage.isNull()) {
            mErrorString = QStringLiteral("No Artwork");
        } else if (mRequestedSize.isValid() and not mRequestedSize.isEmpty()) {
            mImage = mImage.scaled(mRequestedSize, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        }
        Q_EMIT finished();
    }

    QSize mRequestedSize;
    std::shared_ptr<Artwork::Request> mRequest;
    QImage mImage;
    QString mErrorString;
};

} // namespace

ArtworkImageProvider::ArtworkImageProvider(Artwork::Lookup& lookup)
    : mLookup{lookup}
{
}

ArtworkImageProvider::~ArtworkImageProvider() = default;

QQuickImageResponse* ArtworkImageProvider::requestImageResponse(QString const& id, QSize const& requestedSize)
{
    auto const query = QUrlQuery{id.startsWith(QLatin1Char('?')) ? id.mid(1) : id};
    auto const artist = query.queryItemValue(QStringLiteral("artist"), QUrl::FullyDecoded);
    auto const album = query.queryItemValue(QStringLiteral("album"), QUrl::FullyDecoded);
    // QML takes the ownership of the response.
    return new ArtworkImageResponse{mLookup, artist, album, requestedSize}; // NOLINT cppcoreguidelines-owning-memory
}

} // namespace Shell

#include "ArtworkImageProvider.moc"
