// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "MediaServerDouble.h"
#include "InvalidDeviceDescription.h"
#include "PendingSoapCall.h"
#include <QDebug>
#include <QUrl>

namespace MediaServer::Plugin::Doubles
{

std::unique_ptr<UPnPAV::IMediaServer> MediaServerFactory::createMediaServer(const UPnPAV::DeviceDescription &deviceDescription)
{
    Q_UNUSED(deviceDescription)
    ++howOftenCalled;

    return std::make_unique<MediaServer>();
}

MediaServer::MediaServer()
    : UPnPAV::IMediaServer()
{
}

QString MediaServer::name() const noexcept
{
    return "MediaServer";
}

QUrl MediaServer::iconUrl() const noexcept
{
    return QUrl{ "http://localhost:8200/icons/sm.png" };
}

QSharedPointer<UPnPAV::PendingSoapCall> MediaServer::getSortCapabilities() noexcept
{
    return {};
}

QSharedPointer<UPnPAV::PendingSoapCall> MediaServer::browse(const QString &objectId, BrowseFlag browseFlag,
                                                            const QString &filter, const QString &sortCriteria) noexcept
{
    Q_UNUSED(filter)
    Q_UNUSED(sortCriteria)

    lastBrowseRequest.objectId = objectId;
    lastBrowseRequest.browseFlag = browseFlag;

    return QSharedPointer<UPnPAV::PendingSoapCall>{ new UPnPAV::PendingSoapCall{ soapCall } };
}

} // namespace MediaServer::Plugin::Doubles
