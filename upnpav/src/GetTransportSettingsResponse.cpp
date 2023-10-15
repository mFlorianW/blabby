// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "GetTransportSettingsResponse.h"
#include "private/LoggingCategories.h"
#include "private/ResponseReader.h"

namespace UPnPAV
{

GetTransportSettingsResponse::GetTransportSettingsResponse(QString rawMsg,
                                                           ServiceControlPointDefinition scpd,
                                                           SCPDAction action)
    : d{new GetTransportSettingsResponseData{}}
{
    auto reader = ResponseReader{rawMsg, scpd, action};
    QObject::connect(&reader,
                     &ResponseReader::stringValueRead,
                     &reader,
                     [&](QString const &elementName, QString value, ResponseReader::ElementReadResult result) {
                         const auto ok = result == ResponseReader::ElementReadResult::Ok;
                         if (elementName == QStringLiteral("PlayMode") && ok)
                         {
                             d->mPlayMode = convertPlayMode(value).value_or(PlayMode::Normal);
                         }
                         else if (elementName == QStringLiteral("RecQualityMode") && ok)
                         {
                             d->mRecQMode = value;
                         }
                         else if (not ok)
                         {
                             qCCritical(upnpavDevice)
                                 << "Failed to read element" << elementName << "Error:" << static_cast<qint32>(result);
                             return;
                         }
                     });

    const auto result = reader.read();
    if (result != ResponseReader::ReadResult::Ok)
    {
        qCCritical(upnpavDevice) << "Failed to read the GetTransportSettingsResponse. Error:"
                                 << static_cast<qint32>(result) << "Response:" << rawMsg;
        return;
    }
}

GetTransportSettingsResponse::PlayMode GetTransportSettingsResponse::playMode() const noexcept
{
    return d->mPlayMode;
}

const QString &GetTransportSettingsResponse::recQualityMode() const noexcept
{
    return d->mRecQMode;
}

std::optional<GetTransportSettingsResponse::PlayMode> GetTransportSettingsResponse::convertPlayMode(
    QString const &rawMode) noexcept
{
    if (rawMode == QStringLiteral("NORMAL"))
    {
        return PlayMode::Normal;
    }
    else if (rawMode == QStringLiteral("SHUFFLE"))
    {
        return PlayMode::Shuffle;
    }
    else if (rawMode == QStringLiteral("REPEAT_ONE"))
    {
        return PlayMode::RepeatOne;
    }
    else if (rawMode == QStringLiteral("REPEAT_ALL"))
    {
        return PlayMode::RepeatAll;
    }
    else if (rawMode == QStringLiteral("RANDOM"))
    {
        return PlayMode::Random;
    }
    else if (rawMode == QStringLiteral("DIRECT_1"))
    {
        return PlayMode::DirectOne;
    }
    else if (rawMode == QStringLiteral("INTRO"))
    {
        return PlayMode::Intro;
    }
    else
    {
        return std::nullopt;
    }
}

} // namespace UPnPAV
