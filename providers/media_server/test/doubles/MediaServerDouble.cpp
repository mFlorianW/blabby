// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "MediaServerDouble.h"
#include "ContentDirectoryActions.h"
#include "Descriptions.h"
#include "PendingSoapCall.h"
#include <QDebug>
#include <QUrl>

namespace Provider::Doubles
{

std::unique_ptr<UPnPAV::MediaServer> MediaServerFactory::createMediaServer(
    const UPnPAV::DeviceDescription &deviceDescription)
{
    Q_UNUSED(deviceDescription)
    ++howOftenCalled;

    return std::make_unique<MediaServer>();
}

MediaServer::MediaServer()
    : UPnPAV::MediaServer(
          UPnPAV::DeviceDescription{
              "",
              "MediaServerName",
              "",
              "",
              "",
              QVector<UPnPAV::IconDescription>{},
              {UPnPAV::validContentDirectoryDescription(), UPnPAV::validConnectionManagerDescription()},
              {UPnPAV::validContentDirectorySCPD(), UPnPAV::validConnectionManagerSCPD()}},
          QSharedPointer<UPnPAV::SoapMessageTransmitter>(nullptr))
    , soapCall{QSharedPointer<UPnPAV::SoapCallDouble>::create(UPnPAV::validContentDirectorySCPD(), UPnPAV::Browse())}
{
}

MediaServer::~MediaServer() = default;

const QString &MediaServer::name() const noexcept
{
    return mName;
}

const QUrl &MediaServer::iconUrl() const noexcept
{
    return mIconUrl;
}

QScopedPointer<UPnPAV::PendingSoapCall> MediaServer::protocolInfo() noexcept
{
    return QScopedPointer<UPnPAV::PendingSoapCall>{nullptr};
}

QScopedPointer<UPnPAV::PendingSoapCall> MediaServer::currentConnectionIds() noexcept
{
    return QScopedPointer<UPnPAV::PendingSoapCall>{nullptr};
}

QScopedPointer<UPnPAV::PendingSoapCall> MediaServer::currentConnectionInfo(quint32 connectionId) noexcept
{
    Q_UNUSED(connectionId)
    return QScopedPointer<UPnPAV::PendingSoapCall>{nullptr};
}

QSharedPointer<UPnPAV::PendingSoapCall> MediaServer::getSortCapabilities() noexcept
{
    return {};
}

QSharedPointer<UPnPAV::PendingSoapCall> MediaServer::browse(const QString &objectId,
                                                            BrowseFlag browseFlag,
                                                            const QString &filter,
                                                            const QString &sortCriteria) noexcept
{
    Q_UNUSED(filter)
    Q_UNUSED(sortCriteria)

    lastBrowseRequest.objectId = objectId;
    lastBrowseRequest.browseFlag = browseFlag;

    return QSharedPointer<UPnPAV::PendingSoapCall>{new (std::nothrow) UPnPAV::PendingSoapCall{soapCall}};
}

} // namespace Provider::Doubles
