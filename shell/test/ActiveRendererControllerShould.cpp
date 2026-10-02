// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "ActiveRendererControllerShould.hpp"
#include "Descriptions.hpp"
#include "InMemoryRendererStore.hpp"
#include "PositionInfoResponse.hpp"
#include "RendererProvider.hpp"
#include <QSignalSpy>
#include <QTest>

using namespace UPnPAV;
using namespace UPnPAV::Doubles;
using namespace Multimedia;

namespace Shell
{

namespace
{
constexpr auto kitchenUsn = QLatin1StringView{"uuid:kitchen"};
constexpr auto bathroomUsn = QLatin1StringView{"uuid:bathroom"};
} // namespace

ActiveRendererControllerShould::~ActiveRendererControllerShould() = default;

void ActiveRendererControllerShould::init()
{
    mController.reset();
    mModel.reset();
    auto sProvider = std::make_unique<ServiceProviderDouble>();
    sProvider->addDeviceDescription(
        kitchenUsn,
        validRendererDeviceDescription(QStringLiteral("Kitchen"), QString{}, QString{}, kitchenUsn, QString{}));
    sProvider->addDeviceDescription(
        bathroomUsn,
        validRendererDeviceDescription(QStringLiteral("Bathroom"), QString{}, QString{}, bathroomUsn, QString{}));
    mServiceProvider = sProvider.get();
    auto rendererFactory = std::make_unique<MediaRendererDoubleFactory>();
    mRendererFactory = rendererFactory.get();
    auto rProvider = std::make_unique<RendererProvider>(std::make_shared<TestHelper::InMemoryRendererStore>(),
                                                        std::move(sProvider),
                                                        std::move(rendererFactory));
    mModel = std::make_unique<MediaRendererModel>(std::move(rProvider));
    mController = std::make_unique<ActiveRendererController>(*mModel);

    Q_EMIT mServiceProvider->serviceConnected(kitchenUsn);
    Q_EMIT mServiceProvider->serviceConnected(bathroomUsn);
}

void ActiveRendererControllerShould::activate(QString const& name)
{
    for (auto row = 0; row < mModel->rowCount(); ++row) {
        auto const index = mModel->index(row);
        if (mModel->data(index, static_cast<int>(MediaRendererModel::DisplayRole::Name)).toString() == name) {
            mModel->activateRenderer(index);
            return;
        }
    }
    QFAIL("No Renderer with the name found.");
}

MediaRendererDouble* ActiveRendererControllerShould::kitchen() const noexcept
{
    return mRendererFactory->renderer(QStringLiteral("Kitchen"));
}

void ActiveRendererControllerShould::have_no_active_renderer_at_start()
{
    QCOMPARE(mController->property("hasActiveRenderer").toBool(), false);
    QCOMPARE(mController->property("rendererName").toString(), QString{});
    QCOMPARE(mController->property("playbackState").value<Renderer::State>(), Renderer::State::NoMedia);
}

void ActiveRendererControllerShould::follow_the_active_renderer()
{
    auto activeRendererChangedSpy =
        QSignalSpy{mController.get(), &ActiveRendererController::activeRendererChanged};

    activate(QStringLiteral("Kitchen"));

    QCOMPARE(activeRendererChangedSpy.size(), 1);
    QCOMPARE(mController->hasActiveRenderer(), true);
    QCOMPARE(mController->rendererName(), QStringLiteral("Kitchen"));
}

void ActiveRendererControllerShould::give_the_playback_state_of_the_active_renderer()
{
    mRendererFactory->renderer(QStringLiteral("Kitchen"))->setDeviceState(MediaDevice::State::Playing);
    auto playbackStateChangedSpy =
        QSignalSpy{mController.get(), &ActiveRendererController::playbackStateChanged};

    activate(QStringLiteral("Kitchen"));

    QCOMPARE(playbackStateChangedSpy.size(), 1);
    QCOMPARE(mController->playbackState(), Renderer::State::Playing);
}

void ActiveRendererControllerShould::notify_about_a_changed_playback_state_of_the_active_renderer()
{
    activate(QStringLiteral("Kitchen"));
    auto playbackStateChangedSpy =
        QSignalSpy{mController.get(), &ActiveRendererController::playbackStateChanged};

    mRendererFactory->renderer(QStringLiteral("Kitchen"))->setDeviceState(MediaDevice::State::PausedPlayback);

    QCOMPARE(playbackStateChangedSpy.size(), 1);
    QCOMPARE(mController->playbackState(), Renderer::State::Paused);
}

void ActiveRendererControllerShould::follow_a_switch_to_another_active_renderer()
{
    activate(QStringLiteral("Kitchen"));
    mRendererFactory->renderer(QStringLiteral("Bathroom"))->setDeviceState(MediaDevice::State::Stopped);
    auto activeRendererChangedSpy =
        QSignalSpy{mController.get(), &ActiveRendererController::activeRendererChanged};

    activate(QStringLiteral("Bathroom"));

    QCOMPARE(activeRendererChangedSpy.size(), 1);
    QCOMPARE(mController->hasActiveRenderer(), true);
    QCOMPARE(mController->rendererName(), QStringLiteral("Bathroom"));
    QCOMPARE(mController->playbackState(), Renderer::State::Stopped);
}

void ActiveRendererControllerShould::ignore_playback_state_changes_of_the_previous_active_renderer()
{
    activate(QStringLiteral("Kitchen"));
    activate(QStringLiteral("Bathroom"));
    auto playbackStateChangedSpy =
        QSignalSpy{mController.get(), &ActiveRendererController::playbackStateChanged};

    mRendererFactory->renderer(QStringLiteral("Kitchen"))->setDeviceState(MediaDevice::State::Playing);

    QCOMPARE(playbackStateChangedSpy.size(), 0);
    QCOMPARE(mController->playbackState(), Renderer::State::NoMedia);
}

void ActiveRendererControllerShould::report_the_active_renderer_going_offline()
{
    mRendererFactory->renderer(QStringLiteral("Kitchen"))->setDeviceState(MediaDevice::State::Playing);
    activate(QStringLiteral("Kitchen"));
    auto activeRendererChangedSpy =
        QSignalSpy{mController.get(), &ActiveRendererController::activeRendererChanged};
    auto wentOfflineSpy = QSignalSpy{mController.get(), &ActiveRendererController::activeRendererWentOffline};

    Q_EMIT mServiceProvider->serviceDisconnected(kitchenUsn);

    QCOMPARE(wentOfflineSpy.size(), 1);
    QCOMPARE(wentOfflineSpy.at(0).at(0).toString(), QStringLiteral("Kitchen"));
    QCOMPARE(activeRendererChangedSpy.size(), 1);
    QCOMPARE(mController->hasActiveRenderer(), false);
    QCOMPARE(mController->rendererName(), QString{});
    QCOMPARE(mController->playbackState(), Renderer::State::NoMedia);
}

void ActiveRendererControllerShould::track_the_position_of_the_active_renderer_only()
{
    auto* kitchen = mRendererFactory->renderer(QStringLiteral("Kitchen"));
    auto* bathroom = mRendererFactory->renderer(QStringLiteral("Bathroom"));

    activate(QStringLiteral("Kitchen"));
    // A tracked Renderer requests its position info right away.
    QCOMPARE(kitchen->positionInfoCallCount(), 1);
    QCOMPARE(bathroom->positionInfoCallCount(), 0);
    kitchen->finishPositionInfoCall(positionInfoResponse(QStringLiteral("http://192.168.0.3/1.flac")));

    activate(QStringLiteral("Bathroom"));
    QCOMPARE(bathroom->positionInfoCallCount(), 1);
    // An untracked Renderer doesn't request its position info after a Playback State change.
    kitchen->setDeviceState(MediaDevice::State::Stopped);
    QCOMPARE(kitchen->positionInfoCallCount(), 1);
}

void ActiveRendererControllerShould::give_the_current_track_of_the_active_renderer()
{
    mRendererFactory->renderer(QStringLiteral("Kitchen"))
        ->setCurrentTrack(QStringLiteral("http://192.168.0.3/1.flac"),
                          QStringLiteral(R"(<DIDL-Lite xmlns:dc="http://purl.org/dc/elements/1.1/" )"
                                         R"(xmlns:upnp="urn:schemas-upnp-org:metadata-1-0/upnp/">)"
                                         R"(<item id="1" parentID="0"><dc:title>Harbour Lights</dc:title>)"
                                         R"(<upnp:artist>The Quiet Ferries</upnp:artist>)"
                                         R"(<upnp:albumArtURI>http://192.168.0.3/1.jpg</upnp:albumArtURI>)"
                                         R"(<upnp:album>Low Tide Sessions</upnp:album><dc:date>2024-03-01</dc:date>)"
                                         R"(<res protocolInfo="http-get:*:audio/flac:*" bitsPerSample="24" )"
                                         R"(sampleFrequency="96000">http://192.168.0.3/1.flac</res>)"
                                         R"(</item></DIDL-Lite>)"));
    auto currentTrackChangedSpy = QSignalSpy{mController.get(), &ActiveRendererController::currentTrackChanged};

    activate(QStringLiteral("Kitchen"));

    QCOMPARE(currentTrackChangedSpy.size(), 1);
    QCOMPARE(mController->property("trackTitle").toString(), QStringLiteral("Harbour Lights"));
    QCOMPARE(mController->property("trackArtist").toString(), QStringLiteral("The Quiet Ferries"));
    QCOMPARE(mController->property("artworkUrl").toString(), QStringLiteral("http://192.168.0.3/1.jpg"));
    QCOMPARE(mController->property("trackAlbum").toString(), QStringLiteral("Low Tide Sessions"));
    QCOMPARE(mController->property("trackYear").toString(), QStringLiteral("2024"));
    QCOMPARE(mController->property("trackFormat").toString(), QStringLiteral("FLAC · 24-bit / 96 kHz"));

    mRendererFactory->renderer(QStringLiteral("Kitchen"))
        ->setCurrentTrack(QStringLiteral("http://192.168.0.3/Tide.flac"), QString{});
    QCOMPARE(currentTrackChangedSpy.size(), 2);
    QCOMPARE(mController->property("trackTitle").toString(), QStringLiteral("Tide"));
    QCOMPARE(mController->property("trackArtist").toString(), QString{});
}

void ActiveRendererControllerShould::give_no_current_track_without_an_active_renderer()
{
    mRendererFactory->renderer(QStringLiteral("Kitchen"))
        ->setCurrentTrack(QStringLiteral("http://192.168.0.3/Tide.flac"), QString{});

    QCOMPARE(mController->property("trackTitle").toString(), QString{});
    QCOMPARE(mController->property("trackArtist").toString(), QString{});
    QCOMPARE(mController->property("artworkUrl").toString(), QString{});
    QCOMPARE(mController->property("trackAlbum").toString(), QString{});
    QCOMPARE(mController->property("trackYear").toString(), QString{});
    QCOMPARE(mController->property("trackFormat").toString(), QString{});
}

void ActiveRendererControllerShould::give_whether_the_active_renderer_can_pause_and_is_transitioning()
{
    QCOMPARE(mController->property("canPause").toBool(), false);
    kitchen()->setPauseEnabled(true);
    kitchen()->setDeviceState(MediaDevice::State::Playing);
    activate(QStringLiteral("Kitchen"));
    QCOMPARE(mController->property("canPause").toBool(), true);
    auto transitioningChangedSpy = QSignalSpy{mController.get(), &ActiveRendererController::transitioningChanged};

    kitchen()->setDeviceState(MediaDevice::State::Transitioning);

    QCOMPARE(transitioningChangedSpy.size(), 1);
    QCOMPARE(mController->property("transitioning").toBool(), true);
    QCOMPARE(mController->playbackState(), Renderer::State::Playing);
}

void ActiveRendererControllerShould::report_a_failed_control_call_with_the_renderer_name()
{
    kitchen()->setPauseEnabled(true);
    kitchen()->setDeviceState(MediaDevice::State::Playing);
    activate(QStringLiteral("Kitchen"));
    auto controlFailedSpy = QSignalSpy{mController.get(), &ActiveRendererController::controlFailed};
    mModel->activeRenderer()->stop();

    kitchen()->pauseCall()->setErrorState(true);
    Q_EMIT kitchen()->pauseCall()->finished();

    QCOMPARE(controlFailedSpy.size(), 1);
    QCOMPARE(controlFailedSpy.at(0).at(0).toString(), QStringLiteral("Kitchen"));
    QCOMPARE(controlFailedSpy.at(0).at(1).value<Renderer::Action>(), Renderer::Action::Pause);
    QCOMPARE(mController->playbackState(), Renderer::State::Playing);
}

void ActiveRendererControllerShould::give_the_position_and_the_duration_of_the_active_renderer()
{
    kitchen()->setRelTimeSeekEnabled(true);
    kitchen()->setDeviceState(MediaDevice::State::Playing);
    activate(QStringLiteral("Kitchen"));
    auto positionChangedSpy = QSignalSpy{mController.get(), &ActiveRendererController::positionChanged};
    auto durationChangedSpy = QSignalSpy{mController.get(), &ActiveRendererController::durationChanged};
    QCOMPARE(mController->property("canSeek").toBool(), false);

    kitchen()->finishPositionInfoCall(positionInfoResponse(QStringLiteral("http://192.168.0.3/1.flac"),
                                                           QStringLiteral("NOT_IMPLEMENTED"),
                                                           QStringLiteral("0:04:31"),
                                                           QStringLiteral("0:01:42")));

    QCOMPARE(positionChangedSpy.size(), 1);
    QCOMPARE(durationChangedSpy.size(), 1);
    QCOMPARE(mController->property("position").toLongLong(), 102'000);
    QCOMPARE(mController->property("hasDuration").toBool(), true);
    QCOMPARE(mController->property("duration").toLongLong(), 271'000);
    QCOMPARE(mController->property("canSeek").toBool(), true);
}

void ActiveRendererControllerShould::give_no_position_and_duration_without_an_active_renderer()
{
    QCOMPARE(mController->property("position").toLongLong(), 0);
    QCOMPARE(mController->property("hasDuration").toBool(), false);
    QCOMPARE(mController->property("duration").toLongLong(), 0);
    QCOMPARE(mController->property("canSeek").toBool(), false);
}

void ActiveRendererControllerShould::seek_in_the_current_track_of_the_active_renderer()
{
    kitchen()->setRelTimeSeekEnabled(true);
    activate(QStringLiteral("Kitchen"));

    mController->seek(62'000);

    QCOMPARE(kitchen()->seekData().has_value(), true);
    auto const seekData = kitchen()->seekData().value_or(SeekData{});
    QCOMPARE(seekData.mode, MediaDevice::SeekMode::RelTime);
    QCOMPARE(seekData.target, QStringLiteral("0:01:02"));
}

void ActiveRendererControllerShould::give_the_volume_of_the_active_renderer()
{
    QCOMPARE(mController->property("canControlVolume").toBool(), false);
    kitchen()->setVolumeEnabled(true);
    kitchen()->setVolumeRange(VolumeRange{.minimum = 0, .maximum = 60});
    activate(QStringLiteral("Kitchen"));
    auto volumeChangedSpy = QSignalSpy{mController.get(), &ActiveRendererController::volumeChanged};

    Q_EMIT kitchen()->masterVolumeChanged(42);

    QCOMPARE(volumeChangedSpy.size(), 1);
    QCOMPARE(mController->property("volume").toInt(), 42);
    QCOMPARE(mController->property("volumeMinimum").toInt(), 0);
    QCOMPARE(mController->property("volumeMaximum").toInt(), 60);
    QCOMPARE(mController->property("canControlVolume").toBool(), true);
}

void ActiveRendererControllerShould::set_the_volume_of_the_active_renderer()
{
    kitchen()->setVolumeEnabled(true);
    activate(QStringLiteral("Kitchen"));

    mController->setVolume(33);

    QCOMPARE(kitchen()->isSetVolumeCalled(), true);
    QCOMPARE(kitchen()->setVolumeData().volume, 33);
}

void ActiveRendererControllerShould::give_the_mute_of_the_active_renderer()
{
    QCOMPARE(mController->property("canControlMute").toBool(), false);
    kitchen()->setMuteEnabled(true);
    activate(QStringLiteral("Kitchen"));
    auto muteChangedSpy = QSignalSpy{mController.get(), &ActiveRendererController::muteChanged};

    Q_EMIT kitchen()->masterMuteChanged(true);

    QCOMPARE(muteChangedSpy.size(), 1);
    QCOMPARE(mController->property("muted").toBool(), true);
    QCOMPARE(mController->property("canControlMute").toBool(), true);
}

void ActiveRendererControllerShould::set_the_mute_of_the_active_renderer()
{
    kitchen()->setMuteEnabled(true);
    activate(QStringLiteral("Kitchen"));

    mController->setMuted(true);

    QCOMPARE(kitchen()->setMuteData().has_value(), true);
    QCOMPARE(kitchen()->setMuteData().value_or(SetMuteData{}).mute, true);
}

} // namespace Shell

QTEST_MAIN(Shell::ActiveRendererControllerShould)
