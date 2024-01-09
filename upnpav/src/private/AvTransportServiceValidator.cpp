// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "AvTransportServiceValidator.hpp"

namespace UPnPAV
{

AvTransportServiceValidator::AvTransportServiceValidator(DeviceDescription& deviceDescription)
{
    m_serviceName = QStringLiteral("AVTransport");
    m_serviceType = QStringLiteral("urn:schemas-upnp-org:service:AVTransport");
    m_deviceDescription = deviceDescription;

    m_stateVariables = QVector<QString>{
        QStringLiteral("TransportState"),
        QStringLiteral("TransportStatus"),
        QStringLiteral("PlaybackStorageMedium"),
        QStringLiteral("RecordStorageMedium"),
        QStringLiteral("PossiblePlaybackStorageMedia"),
        QStringLiteral("CurrentPlayMode"),
        QStringLiteral("TransportPlaySpeed"),
        QStringLiteral("RecordMediumWriteStatus"),
        QStringLiteral("CurrentRecordQualityMode"),
        QStringLiteral("PossibleRecordQualityModes"),
        QStringLiteral("NumberOfTracks"),
        QStringLiteral("CurrentTrack"),
        QStringLiteral("CurrentTrackDuration"),
        QStringLiteral("CurrentMediaDuration"),
        QStringLiteral("CurrentTrackMetaData"),
        QStringLiteral("CurrentTrackURI"),
        QStringLiteral("AVTransportURI"),
        QStringLiteral("AVTransportURIMetaData"),
        QStringLiteral("NextAVTransportURI"),
        QStringLiteral("NextAVTransportURIMetaData"),
        QStringLiteral("RelativeTimePosition"),
        QStringLiteral("AbsoluteTimePosition"),
        QStringLiteral("RelativeCounterPosition"),
        QStringLiteral("AbsoluteCounterPosition"),
        QStringLiteral("LastChange"),
        QStringLiteral("A_ARG_TYPE_SeekMode"),
        QStringLiteral("A_ARG_TYPE_SeekTarget"),
        QStringLiteral("A_ARG_TYPE_InstanceID"),
    };

    m_actions = QVector<QString>{
        QStringLiteral("SetAVTransportURI"),
        QStringLiteral("GetMediaInfo"),
        QStringLiteral("GetTransportInfo"),
        QStringLiteral("GetPositionInfo"),
        QStringLiteral("GetDeviceCapabilities"),
        QStringLiteral("GetTransportSettings"),
        QStringLiteral("Stop"),
        QStringLiteral("Play"),
        QStringLiteral("Seek"),
        QStringLiteral("Next"),
        QStringLiteral("Previous"),
    };
}
} // namespace UPnPAV
