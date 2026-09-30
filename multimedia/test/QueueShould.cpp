// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "QueueShould.hpp"
#include "Descriptions.hpp"
#include "EventBackendDouble.hpp"
#include "Item.hpp"
#include "PositionInfoResponse.hpp"
#include "SoapBackendDouble.hpp"
#include <QSignalSpy>
#include <QTest>

using namespace UPnPAV;
using namespace UPnPAV::Doubles;

namespace Multimedia
{

namespace
{
Item playable(QString const& title)
{
    return ItemBuilder{}
        .withItemType(ItemType::Playable)
        .withMainText(title)
        .withPlayUrl(QStringLiteral("http://192.168.0.3:8200/MediaItems/%1.mp3").arg(title))
        .withSupportedTypes({Protocol::create(QStringLiteral("http-get:*:audio/mpeg")).value_or(Protocol{})})
        .build();
}

Items album()
{
    return {playable(QStringLiteral("Intro")), playable(QStringLiteral("Harbour")), playable(QStringLiteral("Outro"))};
}

QString uriOf(QString const& title)
{
    return playable(title).playUrl();
}

std::unique_ptr<MediaRendererDouble> createDevice(QString const& name, QString const& udn)
{
    return std::make_unique<MediaRendererDouble>(
        validRendererDeviceDescription(name, QStringLiteral("Denon"), QStringLiteral("HEOS 1"), udn, QString{}),
        QSharedPointer<SoapBackendDouble>::create(),
        QSharedPointer<Doubles::EventBackend>::create());
}
} // namespace

QueueShould::~QueueShould() = default;

void QueueShould::init()
{
    auto device = createDevice(QStringLiteral("Kitchen"), QStringLiteral("uuid:kitchen"));
    mDevice = device.get();
    auto clock = std::make_unique<ClockDouble>();
    mClock = clock.get();
    mRenderer = std::make_shared<Renderer>(std::move(device), std::move(clock));
    mQueue = std::make_unique<Queue>();
    mQueue->setActiveRenderer(mRenderer);
    Q_EMIT mDevice->protocolInfoCall()->finished();
}

void QueueShould::cleanup()
{
    mQueue.reset();
    mRenderer.reset();
}

void QueueShould::report(QString const& uri, QString const& duration, QString const& position)
{
    mDevice->setCurrentTrack(uri, QString{});
    mDevice->setDeviceState(MediaDevice::State::Playing);
    mDevice->finishPositionInfoCall(positionInfoResponse(uri, QString{}, duration, position));
}

void QueueShould::finishEntry(QString const& uri)
{
    report(uri, QStringLiteral("0:03:00"), QStringLiteral("0:02:58"));
    mDevice->reset();
    mDevice->setDeviceState(MediaDevice::State::Stopped);
}

void QueueShould::be_empty_and_idle_at_start()
{
    auto queue = Queue{};

    QCOMPARE(queue.entries(), Items{});
    QCOMPARE(queue.currentIndex(), std::nullopt);
    QCOMPARE(queue.state(), Queue::State::Idle);
    QCOMPARE(queue.lastKnownPosition(), std::chrono::milliseconds{0});
}

void QueueShould::hold_the_playables_and_the_current_entry_after_a_replace()
{
    mQueue->replace(album(), 1);

    QCOMPARE(mQueue->entries(), album());
    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{1});
    QCOMPARE(mQueue->currentEntry(), std::optional{playable(QStringLiteral("Harbour"))});
}

void QueueShould::play_the_start_entry_on_the_active_renderer_and_run_after_a_replace()
{
    mQueue->replace(album(), 1);
    Q_EMIT mDevice->avTransportUriCall()->finished();

    QCOMPARE(mDevice->avTransportUriData().uri, uriOf(QStringLiteral("Harbour")));
    QCOMPARE(mDevice->isPlayCalled(), true);
    QCOMPARE(mQueue->state(), Queue::State::Running);
}

void QueueShould::notify_about_a_replace()
{
    auto aboutToBeReplacedSpy = QSignalSpy{mQueue.get(), &Queue::entriesAboutToBeReplaced};
    auto replacedSpy = QSignalSpy{mQueue.get(), &Queue::entriesReplaced};
    auto currentEntryChangedSpy = QSignalSpy{mQueue.get(), &Queue::currentEntryChanged};
    auto stateChangedSpy = QSignalSpy{mQueue.get(), &Queue::stateChanged};

    mQueue->replace(album(), 0);

    QCOMPARE(aboutToBeReplacedSpy.size(), 1);
    QCOMPARE(replacedSpy.size(), 1);
    QCOMPARE(currentEntryChangedSpy.size(), 1);
    QCOMPARE(stateChangedSpy.size(), 1);
}

void QueueShould::only_hold_the_playables_without_an_active_renderer()
{
    mQueue->setActiveRenderer(nullptr);

    mQueue->replace(album(), 2);

    QCOMPARE(mQueue->entries(), album());
    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{2});
    QCOMPARE(mQueue->state(), Queue::State::Idle);
    QCOMPARE(mDevice->isSetAvTransportUriCalled(), false);
}

void QueueShould::record_the_position_of_the_renderer_while_it_plays_the_current_entry()
{
    mQueue->replace(album(), 1);
    auto lastKnownPositionChangedSpy = QSignalSpy{mQueue.get(), &Queue::lastKnownPositionChanged};

    report(uriOf(QStringLiteral("Harbour")), QStringLiteral("0:03:00"), QStringLiteral("0:01:42"));

    QCOMPARE(mQueue->lastKnownPosition(), std::chrono::milliseconds{102'000});
    QCOMPARE(lastKnownPositionChangedSpy.size(), 1);

    mClock->advance(std::chrono::seconds{1});
    mDevice->finishPositionInfoCall(
        positionInfoResponse(uriOf(QStringLiteral("Harbour")), QString{}, "0:03:00", "0:01:43"));

    QCOMPARE(mQueue->lastKnownPosition(), std::chrono::milliseconds{103'000});
    QCOMPARE(lastKnownPositionChangedSpy.size(), 2);
}

void QueueShould::not_take_the_position_of_the_previous_track_for_the_next_entry()
{
    mQueue->replace(album(), 0);
    finishEntry(uriOf(QStringLiteral("Intro")));

    // The device reports the next entry before a position of it was polled, the Renderer still has the old position.
    mDevice->setCurrentTrack(uriOf(QStringLiteral("Harbour")), QString{});
    mDevice->setDeviceState(MediaDevice::State::Playing);
    mDevice->setDeviceState(MediaDevice::State::Stopped);

    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{1});
    QCOMPARE(mQueue->lastKnownPosition(), std::chrono::milliseconds{0});
}

void QueueShould::not_record_the_position_of_another_current_track()
{
    mQueue->replace(album(), 1);
    report(uriOf(QStringLiteral("Harbour")), QStringLiteral("0:03:00"), QStringLiteral("0:01:42"));

    report(QStringLiteral("http://radio.example/stream.mp3"), QStringLiteral("0:05:00"), QStringLiteral("0:00:10"));

    QCOMPARE(mQueue->lastKnownPosition(), std::chrono::milliseconds{102'000});
}

void QueueShould::play_the_next_entry_when_the_current_entry_finished()
{
    mQueue->replace(album(), 0);
    auto currentEntryChangedSpy = QSignalSpy{mQueue.get(), &Queue::currentEntryChanged};

    finishEntry(uriOf(QStringLiteral("Intro")));

    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{1});
    QCOMPARE(currentEntryChangedSpy.size(), 1);
    QCOMPARE(mDevice->isSetAvTransportUriCalled(), true);
    QCOMPARE(mDevice->avTransportUriData().uri, uriOf(QStringLiteral("Harbour")));
    QCOMPARE(mQueue->lastKnownPosition(), std::chrono::milliseconds{0});
    QCOMPARE(mQueue->state(), Queue::State::Running);
}

void QueueShould::become_idle_after_the_last_entry_finished()
{
    mQueue->replace(album(), 2);
    auto stateChangedSpy = QSignalSpy{mQueue.get(), &Queue::stateChanged};

    finishEntry(uriOf(QStringLiteral("Outro")));

    QCOMPARE(mQueue->state(), Queue::State::Idle);
    QCOMPARE(stateChangedSpy.size(), 1);
    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{2});
    QCOMPARE(mDevice->isSetAvTransportUriCalled(), false);
}

void QueueShould::not_advance_on_a_stop_data()
{
    QTest::addColumn<QString>("uri");
    QTest::addColumn<QString>("duration");
    QTest::addColumn<QString>("position");

    QTest::newRow("mid-entry") << uriOf(QStringLiteral("Intro")) << "0:03:00" << "0:01:00";
    QTest::newRow("just before the end window") << uriOf(QStringLiteral("Intro")) << "0:03:00" << "0:02:56";
    QTest::newRow("stream without duration") << uriOf(QStringLiteral("Intro")) << "0:00:00" << "0:59:59";
    QTest::newRow("another current track") << QStringLiteral("http://other.example/song.mp3") << "0:03:00"
                                           << "0:02:59";
}

void QueueShould::not_advance_on_a_stop()
{
    QFETCH(QString, uri);
    QFETCH(QString, duration);
    QFETCH(QString, position);
    mQueue->replace(album(), 0);
    report(uri, duration, position);
    mDevice->reset();

    mDevice->setDeviceState(MediaDevice::State::Stopped);

    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{0});
    QCOMPARE(mQueue->state(), Queue::State::Running);
    QCOMPARE(mDevice->isSetAvTransportUriCalled(), false);
}

void QueueShould::follow_the_active_renderer_it_is_given()
{
    auto device = createDevice(QStringLiteral("Bathroom"), QStringLiteral("uuid:bathroom"));
    auto* bathroomDevice = device.get();
    auto bathroom = std::make_shared<Renderer>(std::move(device), std::make_unique<ClockDouble>());

    mQueue->setActiveRenderer(bathroom);
    Q_EMIT bathroomDevice->protocolInfoCall()->finished();
    mQueue->replace(album(), 0);

    QCOMPARE(mQueue->activeRenderer(), bathroom);
    QCOMPARE(bathroomDevice->avTransportUriData().uri, uriOf(QStringLiteral("Intro")));
    QCOMPARE(mDevice->isSetAvTransportUriCalled(), false);

    // The previous Active Renderer is not followed anymore.
    mQueue->replace(album(), 0);
    finishEntry(uriOf(QStringLiteral("Intro")));
    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{0});
}

void QueueShould::play_the_entry_at_an_index_from_its_start_and_run()
{
    mQueue->replace(album(), 2);
    finishEntry(uriOf(QStringLiteral("Outro")));
    QCOMPARE(mQueue->state(), Queue::State::Idle);
    auto currentEntryChangedSpy = QSignalSpy{mQueue.get(), &Queue::currentEntryChanged};

    mQueue->play(1);
    Q_EMIT mDevice->avTransportUriCall()->finished();

    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{1});
    QCOMPARE(currentEntryChangedSpy.size(), 1);
    QCOMPARE(mDevice->avTransportUriData().uri, uriOf(QStringLiteral("Harbour")));
    QCOMPARE(mDevice->isPlayCalled(), true);
    QCOMPARE(mQueue->lastKnownPosition(), std::chrono::milliseconds{0});
    QCOMPARE(mQueue->state(), Queue::State::Running);
}

void QueueShould::play_the_current_entry_again_from_its_start()
{
    mQueue->replace(album(), 1);
    report(uriOf(QStringLiteral("Harbour")), QStringLiteral("0:03:00"), QStringLiteral("0:01:42"));
    mDevice->reset();

    mQueue->play(1);

    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{1});
    QCOMPARE(mDevice->avTransportUriData().uri, uriOf(QStringLiteral("Harbour")));
    QCOMPARE(mQueue->lastKnownPosition(), std::chrono::milliseconds{0});
}

void QueueShould::only_make_the_entry_at_an_index_current_without_an_active_renderer()
{
    mQueue->setActiveRenderer(nullptr);
    mQueue->replace(album(), 0);

    mQueue->play(2);

    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{2});
    QCOMPARE(mQueue->state(), Queue::State::Idle);
    QCOMPARE(mDevice->isSetAvTransportUriCalled(), false);
}

void QueueShould::ignore_playing_at_an_invalid_index()
{
    mQueue->replace(album(), 1);
    mDevice->reset();

    mQueue->play(3);
    mQueue->play(-1);

    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{1});
    QCOMPARE(mDevice->isSetAvTransportUriCalled(), false);
}

void QueueShould::tell_whether_the_renderer_plays_the_current_entry()
{
    QCOMPARE(mQueue->playsCurrentEntry(), false);
    mQueue->replace(album(), 1);
    QCOMPARE(mQueue->playsCurrentEntry(), false);

    report(uriOf(QStringLiteral("Harbour")), QStringLiteral("0:03:00"), QStringLiteral("0:00:01"));
    QCOMPARE(mQueue->playsCurrentEntry(), true);

    mDevice->setDeviceState(MediaDevice::State::PausedPlayback);
    QCOMPARE(mQueue->playsCurrentEntry(), false);

    // Another controller took over the Renderer.
    report(QStringLiteral("http://radio.example/stream.mp3"), QStringLiteral("0:00:00"), QStringLiteral("0:00:10"));
    QCOMPARE(mQueue->playsCurrentEntry(), false);
}

void QueueShould::notify_when_the_renderer_starts_or_stops_playing_the_current_entry()
{
    mQueue->replace(album(), 2);
    auto playsCurrentEntryChangedSpy = QSignalSpy{mQueue.get(), &Queue::playsCurrentEntryChanged};

    report(uriOf(QStringLiteral("Outro")), QStringLiteral("0:03:00"), QStringLiteral("0:00:01"));
    QCOMPARE(playsCurrentEntryChangedSpy.size(), 1);

    finishEntry(uriOf(QStringLiteral("Outro")));
    QCOMPARE(mQueue->playsCurrentEntry(), false);
    QCOMPARE(playsCurrentEntryChangedSpy.size(), 2);
}

void QueueShould::append_the_playables_at_the_end_and_keep_the_current_entry()
{
    mQueue->replace(album(), 1);
    mDevice->reset();

    mQueue->append({playable(QStringLiteral("Bonus"))});

    QCOMPARE(mQueue->entries(), album() + Items{playable(QStringLiteral("Bonus"))});
    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{1});
    QCOMPARE(mQueue->state(), Queue::State::Running);
    QCOMPARE(mDevice->isSetAvTransportUriCalled(), false);
}

void QueueShould::notify_about_an_append()
{
    mQueue->replace(album(), 0);
    auto aboutToBeAppendedSpy = QSignalSpy{mQueue.get(), &Queue::entriesAboutToBeAppended};
    auto appendedSpy = QSignalSpy{mQueue.get(), &Queue::entriesAppended};
    auto currentEntryChangedSpy = QSignalSpy{mQueue.get(), &Queue::currentEntryChanged};

    mQueue->append({playable(QStringLiteral("Bonus")), playable(QStringLiteral("Hidden"))});
    mQueue->append({});

    QCOMPARE(aboutToBeAppendedSpy.size(), 1);
    QCOMPARE(aboutToBeAppendedSpy.at(0).at(0).value<qsizetype>(), 3);
    QCOMPARE(aboutToBeAppendedSpy.at(0).at(1).value<qsizetype>(), 4);
    QCOMPARE(appendedSpy.size(), 1);
    QCOMPARE(currentEntryChangedSpy.size(), 0);
}

void QueueShould::append_to_an_empty_queue_without_a_current_entry()
{
    mQueue->append({playable(QStringLiteral("Bonus"))});

    QCOMPARE(mQueue->entries(), Items{playable(QStringLiteral("Bonus"))});
    QCOMPARE(mQueue->currentIndex(), std::nullopt);
    QCOMPARE(mQueue->state(), Queue::State::Idle);
}

} // namespace Multimedia

QTEST_MAIN(Multimedia::QueueShould)
