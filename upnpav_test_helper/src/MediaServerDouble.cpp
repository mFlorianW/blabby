// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MediaServerDouble.hpp"
#include "ContentDirectoryActions.hpp"
#include "Descriptions.hpp"
#include "PendingSoapCall.hpp"
#include <QDebug>
#include <QUrl>

namespace UPnPAV::Doubles
{

std::unique_ptr<UPnPAV::MediaServer> MediaServerFactory::createMediaServer(
    UPnPAV::DeviceDescription const &deviceDescription)
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

QString const &MediaServer::name() const noexcept
{
    return mName;
}

QUrl const &MediaServer::iconUrl() const noexcept
{
    return mIconUrl;
}

void MediaServer::setIconUrl(QString const &iconUrl) noexcept
{
    mIconUrl = iconUrl;
}

std::unique_ptr<UPnPAV::PendingSoapCall> MediaServer::protocolInfo() noexcept
{
    return std::make_unique<UPnPAV::PendingSoapCall>(nullptr);
}

std::unique_ptr<UPnPAV::PendingSoapCall> MediaServer::currentConnectionIds() noexcept
{
    return std::make_unique<UPnPAV::PendingSoapCall>(nullptr);
}

std::unique_ptr<UPnPAV::PendingSoapCall> MediaServer::currentConnectionInfo(quint32 connectionId) noexcept
{
    Q_UNUSED(connectionId)
    return std::make_unique<UPnPAV::PendingSoapCall>(nullptr);
}

std::unique_ptr<UPnPAV::PendingSoapCall> MediaServer::getSortCapabilities() noexcept
{
    return std::make_unique<UPnPAV::PendingSoapCall>(nullptr);
}

std::unique_ptr<UPnPAV::PendingSoapCall> MediaServer::browse(QString const &objectId,
                                                             BrowseFlag browseFlag,
                                                             QString const &filter,
                                                             QString const &sortCriteria) noexcept
{
    Q_UNUSED(filter)
    Q_UNUSED(sortCriteria)

    lastBrowseRequest.objectId = objectId;
    lastBrowseRequest.browseFlag = browseFlag;

    return std::make_unique<UPnPAV::PendingSoapCall>(soapCall);
}

} // namespace UPnPAV::Doubles
