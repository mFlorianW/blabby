// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MediaRenderer.hpp"
#include "DeviceDescription.hpp"
#include "HttpEventBackend.hpp"
#include "HttpSoapBackend.hpp"
#include "InvalidDeviceDescription.hpp"
#include "private/MediaRendererPrivate.hpp"
#include "private/RenderingControlServiceValidator.hpp"
#include "private/SoapMessageGenerator.hpp"

namespace UPnPAV
{

MediaRenderer::MediaRenderer(DeviceDescription desc,
                             QSharedPointer<SoapBackend> msgTransmitter,
                             QSharedPointer<EventBackend> eventBackend)
    : MediaDevice{desc, msgTransmitter, eventBackend}
    , d{std::make_unique<MediaRendererPrivate>(desc, msgTransmitter)}
{
    auto validator = RenderingControlServiceValidator{desc};
    if (!validator.validate()) {
        throw InvalidDeviceDescription(validator.errorMessage());
    }

    d->mRenderingControlService = validator.serviceDescription();
    d->mRenderingControlSCPD = validator.scpd();
}

MediaRenderer::~MediaRenderer() = default;

std::optional<std::unique_ptr<PendingSoapCall>> MediaRenderer::volume(quint32 instanceId,
                                                                      QString const& channel) const noexcept
{
    auto const action = d->mRenderingControlSCPD.action("GetVolume");
    if (action.name().isEmpty()) {
        return std::nullopt;
    }
    auto const instanceIdArg = Argument{.name = "InstanceID", .value = QString::number(instanceId)};
    auto const channelArg = Argument{.name = "Channel", .value = channel};
    auto msgGen = SoapMessageGenerator{};
    auto xmlMessage =
        msgGen.generateXmlMessageBody(action, d->mRenderingControlService.serviceType(), {instanceIdArg, channelArg});
    auto soapCall = d->mSoapMessageTransmitter->sendSoapMessage(d->mRenderingControlService.controlUrl(),
                                                                action.name(),
                                                                d->mRenderingControlService.serviceType(),
                                                                xmlMessage);
    return std::make_unique<PendingSoapCall>(soapCall);
}

std::optional<std::unique_ptr<PendingSoapCall>> MediaRenderer::setVolume(quint32 instanceId,
                                                                         QString const& channel,
                                                                         quint32 volume) noexcept
{
    auto const action = d->mRenderingControlSCPD.action("SetVolume");
    if (action.name().isEmpty()) {
        return std::nullopt;
    }
    auto const instanceIdArg = Argument{.name = "InstanceID", .value = QString::number(instanceId)};
    auto const channelArg = Argument{.name = "Channel", .value = channel};
    auto const volumeArg = Argument{.name = "DesiredVolume", .value = QString::number(volume)};
    auto msgGen = SoapMessageGenerator{};
    auto xmlMessage = msgGen.generateXmlMessageBody(action,
                                                    d->mRenderingControlService.serviceType(),
                                                    {instanceIdArg, channelArg, volumeArg});
    auto soapCall = d->mSoapMessageTransmitter->sendSoapMessage(d->mRenderingControlService.controlUrl(),
                                                                action.name(),
                                                                d->mRenderingControlService.serviceType(),
                                                                xmlMessage);
    return std::make_unique<PendingSoapCall>(soapCall);
}

std::unique_ptr<MediaRenderer> MediaRendererFactory::create(DeviceDescription const& desc)
{
    return std::make_unique<MediaRenderer>(desc,
                                           QSharedPointer<HttpSoapBackend>::create(),
                                           QSharedPointer<HttpEventBackend>::create());
}

} // namespace UPnPAV
