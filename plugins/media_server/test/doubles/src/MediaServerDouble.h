// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "DeviceDescription.h"
#include "IMediaServer.h"
#include "SoapCallDouble.h"
#include <QUrl>

namespace MediaServer::Plugin::Doubles
{

class MediaServerFactory final : public UPnPAV::IMediaServerFactory
{
public:
    quint32 howOftenCalled{ 0 };
    std::unique_ptr<UPnPAV::IMediaServer> createMediaServer(const UPnPAV::DeviceDescription &deviceDescription) override;
};

struct LastBrowseRequest
{
    QString objectId;
    UPnPAV::IMediaServer::BrowseFlag browseFlag;

    bool operator==(const LastBrowseRequest &rhs) const noexcept
    {
        return (objectId == rhs.objectId) && (browseFlag == rhs.browseFlag);
    }

    bool operator!=(const LastBrowseRequest &rhs) const noexcept
    {
        return !operator==(rhs);
    }
};

class MediaServer : public UPnPAV::IMediaServer
{
    Q_DISABLE_COPY_MOVE(MediaServer)
public:
    MediaServer();

    const QString &name() const noexcept override;

    const QUrl &iconUrl() const noexcept override;

    QSharedPointer<UPnPAV::PendingSoapCall> getSortCapabilities() noexcept override;

    QSharedPointer<UPnPAV::PendingSoapCall> browse(const QString &objectId, BrowseFlag browseFlag,
                                                   const QString &filter, const QString &sortCriteria) noexcept override;

public:
    LastBrowseRequest lastBrowseRequest;
    QSharedPointer<Doubles::SoapCallDouble> soapCall{ nullptr };
    QString mName{ "MediaServer" };
    QUrl mIconUrl{ "http://localhost:8200/icons/sm.png" };
};

} // namespace MediaServer::Plugin::Doubles
