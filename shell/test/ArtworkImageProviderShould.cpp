// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "ArtworkImageProviderShould.hpp"
#include <QSignalSpy>
#include <QTest>

namespace Shell
{

namespace
{

constexpr auto salt = QLatin1StringView{"?artist=The%20Quiet%20Ferries&album=Salt%20%26%20Spray"};

} // namespace

LookupDouble::LookupDouble() = default;
LookupDouble::~LookupDouble() = default;

std::shared_ptr<Artwork::Request> LookupDouble::requestArtwork(QString const& artist, QString const& album)
{
    lastArtist = artist;
    lastAlbum = album;
    lastRequest = std::make_shared<Artwork::Request>();
    return lastRequest;
}

ArtworkImageProviderShould::~ArtworkImageProviderShould() = default;

void ArtworkImageProviderShould::init()
{
    mLookup = std::make_unique<LookupDouble>();
    mProvider = std::make_unique<ArtworkImageProvider>(*mLookup);
}

void ArtworkImageProviderShould::request_the_artwork_of_the_album_in_the_url()
{
    auto const response = std::unique_ptr<QQuickImageResponse>{mProvider->requestImageResponse(salt, QSize{})};

    QTRY_VERIFY(mLookup->lastRequest != nullptr);
    QCOMPARE(mLookup->lastArtist, QStringLiteral("The Quiet Ferries"));
    QCOMPARE(mLookup->lastAlbum, QStringLiteral("Salt & Spray"));
}

void ArtworkImageProviderShould::deliver_the_artwork()
{
    auto const response = std::unique_ptr<QQuickImageResponse>{mProvider->requestImageResponse(salt, QSize{50, 50})};
    auto finishedSpy = QSignalSpy{response.get(), &QQuickImageResponse::finished};
    QTRY_VERIFY(mLookup->lastRequest != nullptr);
    auto artwork = QImage{QSize{500, 500}, QImage::Format_RGB32};
    artwork.fill(Qt::red);

    mLookup->lastRequest->finish(artwork);

    QCOMPARE(finishedSpy.size(), 1);
    QCOMPARE(response->errorString(), QString{});
    auto const textureFactory = std::unique_ptr<QQuickTextureFactory>{response->textureFactory()};
    QVERIFY(textureFactory != nullptr);
    QCOMPARE(textureFactory->image().size(), QSize(50, 50));
    QCOMPARE(textureFactory->image().pixelColor(0, 0), QColor{Qt::red});
}

void ArtworkImageProviderShould::fail_without_artwork()
{
    auto const response = std::unique_ptr<QQuickImageResponse>{mProvider->requestImageResponse(salt, QSize{50, 50})};
    auto finishedSpy = QSignalSpy{response.get(), &QQuickImageResponse::finished};
    QTRY_VERIFY(mLookup->lastRequest != nullptr);

    mLookup->lastRequest->finish(QImage{});

    QCOMPARE(finishedSpy.size(), 1);
    QVERIFY(not response->errorString().isEmpty());
    QCOMPARE(response->textureFactory(), nullptr);
}

void ArtworkImageProviderShould::forward_the_cancellation_to_the_lookup()
{
    auto const response = std::unique_ptr<QQuickImageResponse>{mProvider->requestImageResponse(salt, QSize{})};
    QTRY_VERIFY(mLookup->lastRequest != nullptr);

    response->cancel();

    QTRY_COMPARE(mLookup->lastRequest->isCancelled(), true);
}

} // namespace Shell

QTEST_MAIN(Shell::ArtworkImageProviderShould)
