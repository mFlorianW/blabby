// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include "AvTransportActions.hpp"
#include "ConnectionManagerActions.hpp"
#include "Descriptions.hpp"
#include "DeviceDescription.hpp"
#include "MediaRenderer.hpp"
#include "SoapCallDouble.hpp"

namespace UPnPAV::Doubles
{

struct AvTransportUriData
{
    quint32 instanceId = quint32{1234};
    QString uri;
    QString uriMetaData;

    friend bool operator==(AvTransportUriData const &lhs, AvTransportUriData const &rhs)
    {
        return ((lhs.instanceId == rhs.instanceId) and (lhs.uri == rhs.uri) and (lhs.uriMetaData == rhs.uriMetaData));
    }
};

struct PlayData
{
    quint32 instanceId = quint32{1234};

    friend bool operator==(PlayData const &lhs, PlayData const &rhs)
    {
        return ((lhs.instanceId == rhs.instanceId));
    }
};

class MediaRendererDouble : public UPnPAV::MediaRenderer
{
public:
    using UPnPAV::MediaRenderer::MediaRenderer;
    ~MediaRendererDouble() override;
    Q_DISABLE_COPY_MOVE(MediaRendererDouble)
    MediaRendererDouble(DeviceDescription desc, QSharedPointer<SoapMessageTransmitter> transmitter);

    std::unique_ptr<PendingSoapCall> protocolInfo() noexcept override;

    QSharedPointer<SoapCallDouble> protocolInfoCall() const noexcept;

    bool isProtocolInfoCalled() const noexcept;

    std::optional<std::unique_ptr<PendingSoapCall>> setAvTransportUri(quint32 instanceId,
                                                                      QString const &uri,
                                                                      QString const &uriMetaData = QString{
                                                                          ""}) noexcept override;

    bool isSetAvTransportUriCalled() const noexcept;

    AvTransportUriData avTransportUriData() const noexcept;

    QSharedPointer<SoapCallDouble> avTransportUriCall() const noexcept;

    std::optional<std::unique_ptr<PendingSoapCall>> play(quint32 instanceId) override;

    bool isPlayCalled() const noexcept;

    PlayData playData() const noexcept;

    QSharedPointer<SoapCallDouble> playCall() const noexcept;

private:
    QSharedPointer<SoapCallDouble> mProtoInfoCall =
        QSharedPointer<SoapCallDouble>::create(UPnPAV::validConnectionManagerSCPD(), UPnPAV::GetProtocolInfo());
    bool mIsProtocolInfoCalled = false;
    QSharedPointer<SoapCallDouble> mSetAvTransportUriCall =
        QSharedPointer<SoapCallDouble>::create(validAvTranportServiceSCPD(), createSetAVTransportURIAction());
    bool mIsSetAvTranstportUriCalled = false;
    AvTransportUriData mSetAvTransportUriData;
    QSharedPointer<SoapCallDouble> mPlayCall =
        QSharedPointer<SoapCallDouble>::create(validAvTranportServiceSCPD(), createPlayAction());
    bool mIsPlayCalled = false;
    PlayData mPlayData;
};

} // namespace UPnPAV::Doubles
