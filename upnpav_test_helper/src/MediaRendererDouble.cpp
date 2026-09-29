// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MediaRendererDouble.hpp"
#include "ProtocolInfoResponse.hpp"

namespace UPnPAV::Doubles
{

MediaRendererDouble::MediaRendererDouble(DeviceDescription desc,
                                         QSharedPointer<SoapBackend> transmitter,
                                         QSharedPointer<EventBackend> eventBackend)
    : UPnPAV::MediaRenderer{std::move(desc), std::move(transmitter), std::move(eventBackend)}
{
    mProtoInfoCall->setRawMessage(ValidProtoclInfoResponseOfRenderer);
}

MediaRendererDouble::~MediaRendererDouble() = default;

std::unique_ptr<PendingSoapCall> MediaRendererDouble::protocolInfo() noexcept
{
    mIsProtocolInfoCalled = true;
    return std::make_unique<PendingSoapCall>(mProtoInfoCall);
}

QSharedPointer<SoapCallDouble> MediaRendererDouble::protocolInfoCall() const noexcept
{
    return mProtoInfoCall;
}

bool MediaRendererDouble::isProtocolInfoCalled() const noexcept
{
    return mIsProtocolInfoCalled;
}

std::optional<std::unique_ptr<PendingSoapCall>> MediaRendererDouble::setAvTransportUri(
    quint32 instanceId,
    QString const& uri,
    QString const& uriMetaData) noexcept
{
    mIsSetAvTranstportUriCalled = true;
    mSetAvTransportUriData = AvTransportUriData{.instanceId = instanceId, .uri = uri, .uriMetaData = uriMetaData};
    return std::make_unique<PendingSoapCall>(mSetAvTransportUriCall);
}

bool MediaRendererDouble::isSetAvTransportUriCalled() const noexcept
{
    return mIsSetAvTranstportUriCalled;
}

AvTransportUriData MediaRendererDouble::avTransportUriData() const noexcept
{
    return mSetAvTransportUriData;
}

QSharedPointer<SoapCallDouble> MediaRendererDouble::avTransportUriCall() const noexcept
{
    return mSetAvTransportUriCall;
}

std::optional<std::unique_ptr<PendingSoapCall>> MediaRendererDouble::play(quint32 instanceId)
{
    mIsPlayCalled = true;
    mPlayData = PlayData{.instanceId = instanceId};
    return std::make_unique<PendingSoapCall>(mPlayCall);
}

bool MediaRendererDouble::isPlayCalled() const noexcept
{
    return mIsPlayCalled;
}

PlayData MediaRendererDouble::playData() const noexcept
{
    return mPlayData;
}

QSharedPointer<SoapCallDouble> MediaRendererDouble::playCall() const noexcept
{
    return mPlayCall;
}

void MediaRendererDouble::reset() noexcept
{
    // ProtocolInfo
    mIsProtocolInfoCalled = false;
    // AVTransportUri
    mIsSetAvTranstportUriCalled = false;
    mSetAvTransportUriData = {};
    // Play
    mIsPlayCalled = false;
    mPlayData = {};
    // Stop
    mIsStopCalled = false;
    mStopData = {};
    // Pause
    mIsPauseCalled = false;
    mPauseEnabled = false;
    mPauseData = {};
}

MediaDevice::State MediaRendererDouble::state() const noexcept
{
    return mState;
}

void MediaRendererDouble::setDeviceState(MediaDevice::State state) noexcept
{
    if (mState != state) {
        mState = state;
        Q_EMIT stateChanged();
    }
}

StopData MediaRendererDouble::stopData() const noexcept
{
    return mStopData;
}

QSharedPointer<SoapCallDouble> MediaRendererDouble::stopCall() const noexcept
{
    return mStopCall;
}

std::optional<std::unique_ptr<PendingSoapCall>> MediaRendererDouble::stop(quint32 instanceId) noexcept
{
    mIsStopCalled = true;
    mStopData = StopData{.instaneId = instanceId};
    return std::make_unique<PendingSoapCall>(mStopCall);
}

bool MediaRendererDouble::isStopCalled() const noexcept
{
    return mIsStopCalled;
}

PauseData MediaRendererDouble::pauseData() const noexcept
{
    return mPauseData;
}

void MediaRendererDouble::setPauseEnabled(bool enabled) noexcept
{
    mPauseEnabled = enabled;
}

bool MediaRendererDouble::isPauseCalled() const noexcept
{
    return mIsPauseCalled;
}

QSharedPointer<SoapCallDouble> MediaRendererDouble::pauseCall() const noexcept
{
    return mPauseCall;
}

std::optional<std::unique_ptr<PendingSoapCall>> MediaRendererDouble::pause(quint32 instanceId) noexcept
{
    if (not mPauseEnabled) {
        return std::nullopt;
    }

    mIsPauseCalled = true;
    mPauseData = {.instaneId = instanceId};
    return std::make_unique<PendingSoapCall>(mPauseCall);
}

bool MediaRendererDouble::canPause() const noexcept
{
    return mPauseEnabled;
}

VolumeData MediaRendererDouble::volumeData() const noexcept
{
    return mVolumeData;
}

void MediaRendererDouble::setVolumeEnabled(bool enabled) noexcept
{
    mVolumeEnabled = enabled;
}

bool MediaRendererDouble::isVolumeCalled() const noexcept
{
    return mIsVolumeCalled;
}

QSharedPointer<SoapCallDouble> MediaRendererDouble::volumeCall() const noexcept
{
    return mVolumeCall;
}

std::optional<std::unique_ptr<PendingSoapCall>> MediaRendererDouble::volume(quint32 instanceId,
                                                                            QString const& channel) noexcept
{
    if (not mVolumeEnabled) {
        return std::nullopt;
    }
    mVolumeData = {.instanceId = instanceId, .channel = channel};
    mIsVolumeCalled = true;
    return std::make_unique<PendingSoapCall>(mVolumeCall);
}

SetVolumeData MediaRendererDouble::setVolumeData() const noexcept
{
    return mSetVolumeData;
}

bool MediaRendererDouble::isSetVolumeCalled() const noexcept
{
    return mIsSetVolumeCalled;
}

QSharedPointer<SoapCallDouble> MediaRendererDouble::setVolumeCall() const noexcept
{

    return mSetVolumeCall;
}

std::optional<std::unique_ptr<PendingSoapCall>> MediaRendererDouble::setVolume(quint32 instanceId,
                                                                               QString const& channel,
                                                                               quint32 volume) noexcept
{
    if (not mVolumeEnabled) {
        return std::nullopt;
    }

    mSetVolumeData = {.instanceId = instanceId, .channel = channel, .volume = static_cast<quint16>(volume)};
    mIsSetVolumeCalled = true;
    return std::make_unique<PendingSoapCall>(mSetVolumeCall);
}

QString const& MediaRendererDouble::currentTrackUri() const noexcept
{
    return mCurrentTrackUri;
}

QString const& MediaRendererDouble::currentTrackMetaData() const noexcept
{
    return mCurrentTrackMetaData;
}

void MediaRendererDouble::setCurrentTrack(QString const& uri, QString const& metaData) noexcept
{
    if (mCurrentTrackUri != uri or mCurrentTrackMetaData != metaData) {
        mCurrentTrackUri = uri;
        mCurrentTrackMetaData = metaData;
        Q_EMIT currentTrackChanged();
    }
}

std::optional<std::unique_ptr<PendingSoapCall>> MediaRendererDouble::positionInfo(quint32 instanceId)
{
    Q_UNUSED(instanceId)
    ++mPositionInfoCallCount;
    return std::make_unique<PendingSoapCall>(mPositionInfoCall);
}

qsizetype MediaRendererDouble::positionInfoCallCount() const noexcept
{
    return mPositionInfoCallCount;
}

QSharedPointer<SoapCallDouble> MediaRendererDouble::positionInfoCall() const noexcept
{
    return mPositionInfoCall;
}

void MediaRendererDouble::finishPositionInfoCall(QString const& response) noexcept
{
    mPositionInfoCall->setErrorState(false);
    mPositionInfoCall->setRawMessage(response);
    Q_EMIT mPositionInfoCall->finished();
}

void MediaRendererDouble::setRelTimeSeekEnabled(bool enabled) noexcept
{
    mRelTimeSeekEnabled = enabled;
}

bool MediaRendererDouble::canSeek(SeekMode mode) const noexcept
{
    return mRelTimeSeekEnabled and mode == SeekMode::RelTime;
}

std::optional<std::unique_ptr<PendingSoapCall>> MediaRendererDouble::seek(quint32 instanceId,
                                                                          SeekMode mode,
                                                                          QString const& target)
{
    mSeekData = SeekData{.instanceId = instanceId, .mode = mode, .target = target};
    return std::make_unique<PendingSoapCall>(mSeekCall);
}

std::optional<SeekData> MediaRendererDouble::seekData() const noexcept
{
    return mSeekData;
}

QSharedPointer<SoapCallDouble> MediaRendererDouble::seekCall() const noexcept
{
    return mSeekCall;
}

} // namespace UPnPAV::Doubles
