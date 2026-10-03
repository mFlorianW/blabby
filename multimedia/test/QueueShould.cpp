// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "QueueShould.hpp"
#include "Descriptions.hpp"
#include "EventBackendDouble.hpp"
#include "Item.hpp"
#include "PositionInfoResponse.hpp"
#include "SoapBackendDouble.hpp"
#include "TestSource.hpp"
#include <QSignalSpy>
#include <QTest>
#include <QTime>

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

Item container(QString const& title, QString const& path)
{
    return ItemBuilder{}.withItemType(ItemType::Container).withMainText(title).withPath(path).build();
}

/**
 * Gives a Source with an artist whose albums, a single and a nested Container hold the Playables "1" to "6":
 * Artist (a) = [Album A (a1) = [1, 2], 3, Album B (a2) = [4, Disc 2 (a2d) = [5], 6]]
 */
std::shared_ptr<TestHelper::TestSource> createArtistSource()
{
    auto source = std::make_shared<TestHelper::TestSource>(QStringLiteral("NAS"), QString{});
    source->setItems(QStringLiteral("a"),
                     {container(QStringLiteral("Album A"), QStringLiteral("a1")),
                      playable(QStringLiteral("3")),
                      container(QStringLiteral("Album B"), QStringLiteral("a2"))});
    source->setItems(QStringLiteral("a1"), {playable(QStringLiteral("1")), playable(QStringLiteral("2"))});
    source->setItems(QStringLiteral("a2"),
                     {playable(QStringLiteral("4")),
                      container(QStringLiteral("Disc 2"), QStringLiteral("a2d")),
                      playable(QStringLiteral("6"))});
    source->setItems(QStringLiteral("a2d"), {playable(QStringLiteral("5"))});
    return source;
}

Item artist()
{
    return container(QStringLiteral("Artist"), QStringLiteral("a"));
}

Items playables(QStringList const& titles)
{
    auto items = Items{};
    for (auto const& title : titles) {
        items.append(playable(title));
    }
    return items;
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

void QueueShould::reportPlayedFor(QString const& uri, std::chrono::seconds elapsed)
{
    report(uri, QStringLiteral("0:03:00"), QStringLiteral("0:00:00"));
    // The Renderer polls the position again once the clock advanced.
    mClock->advance(elapsed);
    auto const position = QTime{0, 0}.addSecs(static_cast<int>(elapsed.count())).toString(QStringLiteral("H:mm:ss"));
    mDevice->finishPositionInfoCall(positionInfoResponse(uri, QString{}, QStringLiteral("0:03:00"), position));
    mDevice->reset();
}

void QueueShould::makeIdle(qsizetype currentIndex)
{
    mQueue->setActiveRenderer(nullptr);
    mQueue->replace(album(), currentIndex);
    mQueue->setActiveRenderer(mRenderer);
    Q_EMIT mDevice->protocolInfoCall()->finished();
    QCOMPARE(mQueue->state(), Queue::State::Idle);
    mDevice->reset();
}

std::shared_ptr<Renderer> QueueShould::createBathroom()
{
    auto device = createDevice(QStringLiteral("Bathroom"), QStringLiteral("uuid:bathroom"));
    mBathroomDevice = device.get();
    return std::make_shared<Renderer>(std::move(device), std::make_unique<ClockDouble>());
}

void QueueShould::handOverToBathroom(bool seekSupported, QString const& duration, QString const& position)
{
    mQueue->setActiveRenderer(createBathroom());
    mBathroomDevice->setRelTimeSeekEnabled(seekSupported);
    Q_EMIT mBathroomDevice->protocolInfoCall()->finished();
    Q_EMIT mBathroomDevice->avTransportUriCall()->finished();
    mBathroomDevice->setCurrentTrack(uriOf(QStringLiteral("Harbour")), QString{});
    mBathroomDevice->setDeviceState(MediaDevice::State::Playing);
    mBathroomDevice->finishPositionInfoCall(
        positionInfoResponse(uriOf(QStringLiteral("Harbour")), QString{}, duration, position));
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

void QueueShould::play_the_following_entry_and_run_on_next()
{
    mQueue->replace(album(), 0);
    reportPlayedFor(uriOf(QStringLiteral("Intro")), std::chrono::seconds{42});
    auto currentEntryChangedSpy = QSignalSpy{mQueue.get(), &Queue::currentEntryChanged};

    mQueue->next();
    Q_EMIT mDevice->avTransportUriCall()->finished();

    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{1});
    QCOMPARE(currentEntryChangedSpy.size(), 1);
    QCOMPARE(mDevice->avTransportUriData().uri, uriOf(QStringLiteral("Harbour")));
    QCOMPARE(mDevice->isPlayCalled(), true);
    QCOMPARE(mQueue->lastKnownPosition(), std::chrono::milliseconds{0});
    QCOMPARE(mQueue->state(), Queue::State::Running);
}

void QueueShould::tell_whether_previous_and_next_are_available()
{
    QCOMPARE(mQueue->hasPrevious(), false);
    QCOMPARE(mQueue->hasNext(), false);

    mQueue->replace(album(), 0);
    QCOMPARE(mQueue->hasPrevious(), true);
    QCOMPARE(mQueue->hasNext(), true);

    mQueue->play(2);
    QCOMPARE(mQueue->hasPrevious(), true);
    QCOMPARE(mQueue->hasNext(), false);
}

void QueueShould::ignore_next_on_the_last_entry()
{
    mQueue->replace(album(), 2);
    mDevice->reset();

    mQueue->next();

    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{2});
    QCOMPARE(mDevice->isSetAvTransportUriCalled(), false);
}

void QueueShould::play_the_preceding_entry_on_previous_within_the_first_3_seconds()
{
    mDevice->setRelTimeSeekEnabled(true);
    mQueue->replace(album(), 1);
    reportPlayedFor(uriOf(QStringLiteral("Harbour")), std::chrono::seconds{3});

    mQueue->previous();
    Q_EMIT mDevice->avTransportUriCall()->finished();

    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{0});
    QCOMPARE(mDevice->avTransportUriData().uri, uriOf(QStringLiteral("Intro")));
    QCOMPARE(mDevice->isPlayCalled(), true);
    QCOMPARE(mDevice->seekData(), std::nullopt);
    QCOMPARE(mQueue->state(), Queue::State::Running);
}

void QueueShould::restart_the_current_entry_on_previous_after_3_seconds()
{
    mDevice->setRelTimeSeekEnabled(true);
    mQueue->replace(album(), 1);
    reportPlayedFor(uriOf(QStringLiteral("Harbour")), std::chrono::seconds{4});
    auto currentEntryChangedSpy = QSignalSpy{mQueue.get(), &Queue::currentEntryChanged};

    mQueue->previous();

    auto const expected = SeekData{.instanceId = 0, .mode = MediaDevice::SeekMode::RelTime, .target = "0:00:00"};
    QCOMPARE(mDevice->seekData(), std::optional{expected});
    QCOMPARE(mDevice->isSetAvTransportUriCalled(), false);
    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{1});
    QCOMPARE(currentEntryChangedSpy.size(), 0);
    QCOMPARE(mQueue->lastKnownPosition(), std::chrono::milliseconds{0});
    QCOMPARE(mQueue->state(), Queue::State::Running);
}

void QueueShould::play_the_preceding_entry_on_a_second_previous_right_after_a_restart()
{
    mDevice->setRelTimeSeekEnabled(true);
    mQueue->replace(album(), 1);
    reportPlayedFor(uriOf(QStringLiteral("Harbour")), std::chrono::seconds{80});

    mQueue->previous();
    mQueue->previous();

    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{0});
    QCOMPARE(mDevice->avTransportUriData().uri, uriOf(QStringLiteral("Intro")));
}

void QueueShould::restart_the_current_entry_on_previous_by_loading_it_again_without_seek_support()
{
    mQueue->replace(album(), 1);
    reportPlayedFor(uriOf(QStringLiteral("Harbour")), std::chrono::seconds{80});

    mQueue->previous();
    Q_EMIT mDevice->avTransportUriCall()->finished();

    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{1});
    QCOMPARE(mDevice->avTransportUriData().uri, uriOf(QStringLiteral("Harbour")));
    QCOMPARE(mDevice->isPlayCalled(), true);
    QCOMPARE(mQueue->lastKnownPosition(), std::chrono::milliseconds{0});
    QCOMPARE(mQueue->state(), Queue::State::Running);
}

void QueueShould::restart_the_first_entry_on_previous()
{
    mDevice->setRelTimeSeekEnabled(true);
    mQueue->replace(album(), 0);
    reportPlayedFor(uriOf(QStringLiteral("Intro")), std::chrono::seconds{1});

    mQueue->previous();

    auto const expected = SeekData{.instanceId = 0, .mode = MediaDevice::SeekMode::RelTime, .target = "0:00:00"};
    QCOMPARE(mDevice->seekData(), std::optional{expected});
    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{0});
}

void QueueShould::start_the_new_current_entry_of_an_idle_queue_data()
{
    QTest::addColumn<bool>("next");
    QTest::addColumn<qsizetype>("expectedIndex");
    QTest::addColumn<QString>("expectedUri");

    QTest::newRow("next") << true << qsizetype{2} << uriOf(QStringLiteral("Outro"));
    QTest::newRow("previous") << false << qsizetype{0} << uriOf(QStringLiteral("Intro"));
}

void QueueShould::start_the_new_current_entry_of_an_idle_queue()
{
    QFETCH(bool, next);
    QFETCH(qsizetype, expectedIndex);
    QFETCH(QString, expectedUri);
    makeIdle(1);

    next ? mQueue->next() : mQueue->previous();
    Q_EMIT mDevice->avTransportUriCall()->finished();

    QCOMPARE(mQueue->currentIndex(), std::optional{expectedIndex});
    QCOMPARE(mDevice->avTransportUriData().uri, expectedUri);
    QCOMPARE(mDevice->isPlayCalled(), true);
    QCOMPARE(mQueue->state(), Queue::State::Running);
}

void QueueShould::take_the_renderer_back_from_another_controller_data()
{
    start_the_new_current_entry_of_an_idle_queue_data();
}

void QueueShould::take_the_renderer_back_from_another_controller()
{
    QFETCH(bool, next);
    QFETCH(qsizetype, expectedIndex);
    QFETCH(QString, expectedUri);
    mQueue->replace(album(), 1);
    reportPlayedFor(uriOf(QStringLiteral("Harbour")), std::chrono::seconds{2});
    // Another controller took over the Renderer.
    report(QStringLiteral("http://radio.example/stream.mp3"), QStringLiteral("0:00:00"), QStringLiteral("0:00:10"));
    mDevice->reset();

    next ? mQueue->next() : mQueue->previous();
    Q_EMIT mDevice->avTransportUriCall()->finished();

    QCOMPARE(mQueue->currentIndex(), std::optional{expectedIndex});
    QCOMPARE(mDevice->avTransportUriData().uri, expectedUri);
    QCOMPARE(mDevice->isPlayCalled(), true);
    QCOMPARE(mQueue->state(), Queue::State::Running);
}

void QueueShould::restart_the_current_entry_on_previous_by_loading_it_again_after_another_controller_took_over()
{
    mDevice->setRelTimeSeekEnabled(true);
    mQueue->replace(album(), 1);
    reportPlayedFor(uriOf(QStringLiteral("Harbour")), std::chrono::seconds{80});
    report(QStringLiteral("http://other.example/song.mp3"), QStringLiteral("0:04:00"), QStringLiteral("0:00:10"));
    mDevice->reset();

    mQueue->previous();

    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{1});
    QCOMPARE(mDevice->avTransportUriData().uri, uriOf(QStringLiteral("Harbour")));
    QCOMPARE(mDevice->seekData(), std::nullopt);
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
    auto aboutToBeAppendedSpy = QSignalSpy{mQueue.get(), &Queue::entriesAboutToBeInserted};
    auto appendedSpy = QSignalSpy{mQueue.get(), &Queue::entriesInserted};
    auto currentEntryChangedSpy = QSignalSpy{mQueue.get(), &Queue::currentEntryChanged};

    mQueue->append({playable(QStringLiteral("Bonus")), playable(QStringLiteral("Hidden"))});
    mQueue->append({});

    QCOMPARE(aboutToBeAppendedSpy.size(), 1);
    QCOMPARE(aboutToBeAppendedSpy.at(0).at(0).value<qsizetype>(), 3);
    QCOMPARE(aboutToBeAppendedSpy.at(0).at(1).value<qsizetype>(), 4);
    QCOMPARE(appendedSpy.size(), 1);
    QCOMPARE(currentEntryChangedSpy.size(), 0);
}

void QueueShould::make_the_first_added_playable_current_on_an_empty_queue_data()
{
    QTest::addColumn<bool>("playNext");

    QTest::newRow("play next") << true;
    QTest::newRow("add to the end") << false;
}

void QueueShould::make_the_first_added_playable_current_on_an_empty_queue()
{
    QFETCH(bool, playNext);
    auto currentEntryChangedSpy = QSignalSpy{mQueue.get(), &Queue::currentEntryChanged};

    if (playNext) {
        mQueue->playNext(playables({QStringLiteral("Bonus"), QStringLiteral("Hidden")}));
    } else {
        mQueue->append(playables({QStringLiteral("Bonus"), QStringLiteral("Hidden")}));
    }

    QCOMPARE(mQueue->entries(), playables({QStringLiteral("Bonus"), QStringLiteral("Hidden")}));
    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{0});
    QCOMPARE(currentEntryChangedSpy.size(), 1);
    QCOMPARE(mQueue->state(), Queue::State::Idle);
    QCOMPARE(mDevice->isSetAvTransportUriCalled(), false);
}

void QueueShould::insert_the_playables_after_the_current_entry_on_play_next()
{
    mQueue->replace(album(), 0);
    mDevice->reset();

    mQueue->playNext(playables({QStringLiteral("Bonus"), QStringLiteral("Hidden")}));

    QCOMPARE(mQueue->entries(),
             playables({QStringLiteral("Intro"),
                        QStringLiteral("Bonus"),
                        QStringLiteral("Hidden"),
                        QStringLiteral("Harbour"),
                        QStringLiteral("Outro")}));
    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{0});
    QCOMPARE(mQueue->state(), Queue::State::Running);
    QCOMPARE(mDevice->isSetAvTransportUriCalled(), false);
}

void QueueShould::notify_about_play_next()
{
    mQueue->replace(album(), 1);
    auto aboutToBeInsertedSpy = QSignalSpy{mQueue.get(), &Queue::entriesAboutToBeInserted};
    auto insertedSpy = QSignalSpy{mQueue.get(), &Queue::entriesInserted};
    auto currentEntryChangedSpy = QSignalSpy{mQueue.get(), &Queue::currentEntryChanged};

    mQueue->playNext(playables({QStringLiteral("Bonus"), QStringLiteral("Hidden")}));
    mQueue->playNext({});

    QCOMPARE(aboutToBeInsertedSpy.size(), 1);
    QCOMPARE(aboutToBeInsertedSpy.at(0).at(0).value<qsizetype>(), 2);
    QCOMPARE(aboutToBeInsertedSpy.at(0).at(1).value<qsizetype>(), 3);
    QCOMPARE(insertedSpy.size(), 1);
    QCOMPARE(currentEntryChangedSpy.size(), 0);
}

void QueueShould::append_on_play_next_without_a_current_entry()
{
    mQueue->replace(album(), 2);
    // Removing the last entry while it's current leaves the entries before it without a Current Entry.
    mQueue->remove(2);

    mQueue->playNext({playable(QStringLiteral("Bonus"))});

    QCOMPARE(mQueue->entries(),
             playables({QStringLiteral("Intro"), QStringLiteral("Harbour"), QStringLiteral("Bonus")}));
    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{2});
    QCOMPARE(mQueue->state(), Queue::State::Idle);
}

void QueueShould::add_a_playable_of_a_source_without_collecting_it_data()
{
    QTest::addColumn<bool>("playNext");
    QTest::addColumn<Items>("expectedEntries");

    QTest::newRow("play next") << true
                               << playables({QStringLiteral("Intro"),
                                             QStringLiteral("Bonus"),
                                             QStringLiteral("Harbour"),
                                             QStringLiteral("Outro")});
    QTest::newRow("add to the end") << false
                                    << playables({QStringLiteral("Intro"),
                                                  QStringLiteral("Harbour"),
                                                  QStringLiteral("Outro"),
                                                  QStringLiteral("Bonus")});
}

void QueueShould::add_a_playable_of_a_source_without_collecting_it()
{
    QFETCH(bool, playNext);
    QFETCH(Items, expectedEntries);
    auto const source = createArtistSource();
    mQueue->replace(album(), 0);

    if (playNext) {
        mQueue->playNext(source, playable(QStringLiteral("Bonus")));
    } else {
        mQueue->append(source, playable(QStringLiteral("Bonus")));
    }

    QCOMPARE(mQueue->entries(), expectedEntries);
    QCOMPARE(mQueue->isCollecting(), false);
    QCOMPARE(source->browsedPageCount(), 0);
}

void QueueShould::collect_the_playables_of_a_container_depth_first_in_source_order_data()
{
    QTest::addColumn<bool>("playNext");
    QTest::addColumn<Items>("expectedEntries");

    auto const collected = playables({QStringLiteral("1"),
                                      QStringLiteral("2"),
                                      QStringLiteral("3"),
                                      QStringLiteral("4"),
                                      QStringLiteral("5"),
                                      QStringLiteral("6")});
    QTest::newRow("play next") << true
                               << Items{playable(QStringLiteral("Intro"))} + collected +
                                      playables({QStringLiteral("Harbour"), QStringLiteral("Outro")});
    QTest::newRow("add to the end") << false << album() + collected;
}

void QueueShould::collect_the_playables_of_a_container_depth_first_in_source_order()
{
    QFETCH(bool, playNext);
    QFETCH(Items, expectedEntries);
    auto const source = createArtistSource();
    mQueue->replace(album(), 0);
    mDevice->reset();

    if (playNext) {
        mQueue->playNext(source, artist());
    } else {
        mQueue->append(source, artist());
    }

    QCOMPARE(mQueue->entries(), expectedEntries);
    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{0});
    QCOMPARE(mQueue->isCollecting(), false);
    QCOMPARE(mDevice->isSetAvTransportUriCalled(), false);
}

void QueueShould::collect_a_container_over_several_pages()
{
    auto const source = createArtistSource();
    source->setItems(QStringLiteral("a1"),
                     playables({QStringLiteral("1a"),
                                QStringLiteral("1b"),
                                QStringLiteral("1c"),
                                QStringLiteral("1d"),
                                QStringLiteral("1e")}));
    source->setPageSize(2);

    mQueue->append(source, artist());

    QCOMPARE(mQueue->entries(),
             playables({QStringLiteral("1a"),
                        QStringLiteral("1b"),
                        QStringLiteral("1c"),
                        QStringLiteral("1d"),
                        QStringLiteral("1e"),
                        QStringLiteral("3"),
                        QStringLiteral("4"),
                        QStringLiteral("5"),
                        QStringLiteral("6")}));
    // Artist: 2 pages, Album A: 3 pages, Album B: 2 pages, Disc 2: 1 page.
    QCOMPARE(source->browsedPageCount(), 8);
}

void QueueShould::collect_a_container_that_contains_itself_only_once()
{
    auto const source = createArtistSource();
    // A MediaServer may reference a Container inside itself, e.g. through a playlist.
    source->setItems(QStringLiteral("a1"), {playable(QStringLiteral("1")), artist(), playable(QStringLiteral("2"))});

    mQueue->append(source, artist());

    QCOMPARE(mQueue->entries(),
             playables({QStringLiteral("1"),
                        QStringLiteral("2"),
                        QStringLiteral("3"),
                        QStringLiteral("4"),
                        QStringLiteral("5"),
                        QStringLiteral("6")}));
    QCOMPARE(mQueue->isCollecting(), false);
}

void QueueShould::change_only_when_the_collection_completes()
{
    auto const source = createArtistSource();
    source->setHoldPages(true);
    mQueue->replace(album(), 0);
    auto collectingChangedSpy = QSignalSpy{mQueue.get(), &Queue::collectingChanged};
    auto aboutToBeInsertedSpy = QSignalSpy{mQueue.get(), &Queue::entriesAboutToBeInserted};

    mQueue->append(source, artist());
    QCOMPARE(mQueue->isCollecting(), true);
    QCOMPARE(mQueue->collectedContainer(), std::optional{artist()});
    QCOMPARE(collectingChangedSpy.size(), 1);
    // Artist, Album A, Album B and Disc 2.
    for (auto page = 0; page < 3; ++page) {
        source->finishPendingPage();
        QCOMPARE(mQueue->entries(), album());
        QCOMPARE(aboutToBeInsertedSpy.size(), 0);
    }
    source->finishPendingPage();

    QCOMPARE(aboutToBeInsertedSpy.size(), 1);
    QCOMPARE(mQueue->entries().size(), 9);
    QCOMPARE(mQueue->isCollecting(), false);
    QCOMPARE(mQueue->collectedContainer(), std::nullopt);
    QCOMPARE(collectingChangedSpy.size(), 2);
}

void QueueShould::insert_a_collected_container_after_the_current_entry_at_completion()
{
    auto const source = createArtistSource();
    source->setItems(QStringLiteral("a"), playables({QStringLiteral("1"), QStringLiteral("2")}));
    source->setHoldPages(true);
    mQueue->replace(album(), 0);

    mQueue->playNext(source, artist());
    mQueue->play(1);
    source->finishPendingPage();

    QCOMPARE(mQueue->entries(),
             playables({QStringLiteral("Intro"),
                        QStringLiteral("Harbour"),
                        QStringLiteral("1"),
                        QStringLiteral("2"),
                        QStringLiteral("Outro")}));
}

void QueueShould::leave_the_queue_unchanged_and_report_a_failed_collection()
{
    auto const source = createArtistSource();
    source->setHoldPages(true);
    mQueue->replace(album(), 0);
    auto collectionFailedSpy = QSignalSpy{mQueue.get(), &Queue::collectionFailed};

    mQueue->append(source, artist());
    source->finishPendingPage();
    source->failPendingPage();

    QCOMPARE(collectionFailedSpy.size(), 1);
    QCOMPARE(collectionFailedSpy.at(0).at(0).value<Item>(), artist());
    QCOMPARE(mQueue->entries(), album());
    QCOMPARE(mQueue->isCollecting(), false);
    QCOMPARE(source->pendingPageCount(), 0);
    QCOMPARE(source->browsedPageCount(), 2);
}

void QueueShould::cancel_a_collection()
{
    auto const source = createArtistSource();
    source->setHoldPages(true);
    mQueue->replace(album(), 0);
    auto collectionFailedSpy = QSignalSpy{mQueue.get(), &Queue::collectionFailed};

    mQueue->append(source, artist());
    source->finishPendingPage();
    auto collectingChangedSpy = QSignalSpy{mQueue.get(), &Queue::collectingChanged};
    mQueue->cancelCollection();

    QCOMPARE(mQueue->isCollecting(), false);
    QCOMPARE(collectingChangedSpy.size(), 1);
    QCOMPARE(source->pendingPageCount(), 0);
    QCOMPARE(collectionFailedSpy.size(), 0);
    QCOMPARE(mQueue->entries(), album());
}

void QueueShould::cancel_a_collection_by_a_new_one()
{
    auto const source = createArtistSource();
    source->setHoldPages(true);
    mQueue->replace(album(), 0);

    mQueue->append(source, artist());
    mQueue->append(source, container(QStringLiteral("Album A"), QStringLiteral("a1")));
    QCOMPARE(source->pendingPageCount(), 1);
    source->finishPendingPage();

    QCOMPARE(mQueue->entries(), album() + playables({QStringLiteral("1"), QStringLiteral("2")}));
    QCOMPARE(mQueue->isCollecting(), false);
}

void QueueShould::abort_a_collection_when_its_source_disappears()
{
    auto source = createArtistSource();
    source->setHoldPages(true);
    mQueue->replace(album(), 0);
    auto collectionFailedSpy = QSignalSpy{mQueue.get(), &Queue::collectionFailed};

    mQueue->append(source, artist());
    auto collectingChangedSpy = QSignalSpy{mQueue.get(), &Queue::collectingChanged};
    source.reset();

    QCOMPARE(mQueue->isCollecting(), false);
    QCOMPARE(collectingChangedSpy.size(), 1);
    QCOMPARE(collectionFailedSpy.size(), 0);
    QCOMPARE(mQueue->entries(), album());
}

void QueueShould::remove_an_entry_that_is_not_current_without_affecting_playback_data()
{
    QTest::addColumn<qsizetype>("removedIndex");
    QTest::addColumn<Items>("expectedEntries");
    QTest::addColumn<qsizetype>("expectedCurrentIndex");

    QTest::newRow("before the Current Entry")
        << qsizetype{0} << Items{playable(QStringLiteral("Harbour")), playable(QStringLiteral("Outro"))}
        << qsizetype{0};
    QTest::newRow("after the Current Entry")
        << qsizetype{2} << Items{playable(QStringLiteral("Intro")), playable(QStringLiteral("Harbour"))}
        << qsizetype{1};
}

void QueueShould::remove_an_entry_that_is_not_current_without_affecting_playback()
{
    QFETCH(qsizetype, removedIndex);
    QFETCH(Items, expectedEntries);
    QFETCH(qsizetype, expectedCurrentIndex);
    mQueue->replace(album(), 1);
    report(uriOf(QStringLiteral("Harbour")), QStringLiteral("0:03:00"), QStringLiteral("0:01:42"));
    mDevice->reset();
    auto currentEntryChangedSpy = QSignalSpy{mQueue.get(), &Queue::currentEntryChanged};

    mQueue->remove(removedIndex);

    QCOMPARE(mQueue->entries(), expectedEntries);
    QCOMPARE(mQueue->currentIndex(), std::optional{expectedCurrentIndex});
    QCOMPARE(mQueue->currentEntry(), std::optional{playable(QStringLiteral("Harbour"))});
    QCOMPARE(currentEntryChangedSpy.size(), 0);
    QCOMPARE(mQueue->lastKnownPosition(), std::chrono::milliseconds{102'000});
    QCOMPARE(mQueue->state(), Queue::State::Running);
    QCOMPARE(mDevice->isSetAvTransportUriCalled(), false);
    QCOMPARE(mDevice->isStopCalled(), false);
}

void QueueShould::notify_about_a_remove()
{
    mQueue->replace(album(), 1);
    auto aboutToBeRemovedSpy = QSignalSpy{mQueue.get(), &Queue::entryAboutToBeRemoved};
    auto removedSpy = QSignalSpy{mQueue.get(), &Queue::entryRemoved};

    mQueue->remove(2);

    QCOMPARE(aboutToBeRemovedSpy.size(), 1);
    QCOMPARE(aboutToBeRemovedSpy.at(0).at(0).value<qsizetype>(), 2);
    QCOMPARE(removedSpy.size(), 1);
}

void QueueShould::play_the_next_entry_when_the_current_entry_of_a_running_queue_is_removed()
{
    mQueue->replace(album(), 1);
    report(uriOf(QStringLiteral("Harbour")), QStringLiteral("0:03:00"), QStringLiteral("0:01:42"));
    mDevice->reset();
    auto currentEntryChangedSpy = QSignalSpy{mQueue.get(), &Queue::currentEntryChanged};

    mQueue->remove(1);
    Q_EMIT mDevice->avTransportUriCall()->finished();

    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{1});
    QCOMPARE(mQueue->currentEntry(), std::optional{playable(QStringLiteral("Outro"))});
    QCOMPARE(currentEntryChangedSpy.size(), 1);
    QCOMPARE(mDevice->avTransportUriData().uri, uriOf(QStringLiteral("Outro")));
    QCOMPARE(mDevice->isPlayCalled(), true);
    QCOMPARE(mQueue->lastKnownPosition(), std::chrono::milliseconds{0});
    QCOMPARE(mQueue->state(), Queue::State::Running);
}

void QueueShould::make_the_next_entry_current_when_the_current_entry_of_an_idle_queue_is_removed()
{
    makeIdle(0);
    auto currentEntryChangedSpy = QSignalSpy{mQueue.get(), &Queue::currentEntryChanged};

    mQueue->remove(0);

    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{0});
    QCOMPARE(mQueue->currentEntry(), std::optional{playable(QStringLiteral("Harbour"))});
    QCOMPARE(currentEntryChangedSpy.size(), 1);
    QCOMPARE(mQueue->state(), Queue::State::Idle);
    QCOMPARE(mDevice->isSetAvTransportUriCalled(), false);
}

void QueueShould::stop_and_become_idle_when_the_last_remaining_current_entry_is_removed_data()
{
    QTest::addColumn<Items>("entries");
    QTest::addColumn<qsizetype>("currentIndex");
    QTest::addColumn<Items>("expectedEntries");

    QTest::newRow("the only entry") << Items{playable(QStringLiteral("Intro"))} << qsizetype{0} << Items{};
    QTest::newRow("the last entry") << album() << qsizetype{2}
                                    << Items{playable(QStringLiteral("Intro")), playable(QStringLiteral("Harbour"))};
}

void QueueShould::stop_and_become_idle_when_the_last_remaining_current_entry_is_removed()
{
    QFETCH(Items, entries);
    QFETCH(qsizetype, currentIndex);
    QFETCH(Items, expectedEntries);
    mQueue->replace(entries, currentIndex);
    auto const uri = entries.at(currentIndex).playUrl();
    report(uri, QStringLiteral("0:03:00"), QStringLiteral("0:01:42"));
    mDevice->reset();
    auto currentEntryChangedSpy = QSignalSpy{mQueue.get(), &Queue::currentEntryChanged};

    mQueue->remove(currentIndex);

    QCOMPARE(mQueue->entries(), expectedEntries);
    QCOMPARE(mQueue->currentIndex(), std::nullopt);
    QCOMPARE(currentEntryChangedSpy.size(), 1);
    QCOMPARE(mQueue->lastKnownPosition(), std::chrono::milliseconds{0});
    QCOMPARE(mQueue->state(), Queue::State::Idle);
    QCOMPARE(mDevice->isStopCalled(), true);
    QCOMPARE(mDevice->isSetAvTransportUriCalled(), false);
}

void QueueShould::not_stop_another_controller_when_the_last_remaining_current_entry_is_removed()
{
    mQueue->replace(album(), 2);
    // Another controller took over the Renderer.
    report(QStringLiteral("http://radio.example/stream.mp3"), QStringLiteral("0:00:00"), QStringLiteral("0:00:10"));
    mDevice->reset();

    mQueue->remove(2);

    QCOMPARE(mQueue->currentIndex(), std::nullopt);
    QCOMPARE(mQueue->state(), Queue::State::Idle);
    QCOMPARE(mDevice->isStopCalled(), false);
}

void QueueShould::ignore_removing_at_an_invalid_index()
{
    mQueue->replace(album(), 1);
    auto aboutToBeRemovedSpy = QSignalSpy{mQueue.get(), &Queue::entryAboutToBeRemoved};

    mQueue->remove(3);
    mQueue->remove(-1);

    QCOMPARE(mQueue->entries(), album());
    QCOMPARE(aboutToBeRemovedSpy.size(), 0);
}

void QueueShould::move_entries_without_interrupting_playback_data()
{
    QTest::addColumn<qsizetype>("from");
    QTest::addColumn<qsizetype>("to");
    QTest::addColumn<QStringList>("expectedTitles");
    QTest::addColumn<qsizetype>("expectedCurrentIndex");

    QTest::newRow("the Current Entry down")
        << qsizetype{1} << qsizetype{2} << QStringList{"Intro", "Outro", "Harbour"} << qsizetype{2};
    QTest::newRow("the Current Entry up")
        << qsizetype{1} << qsizetype{0} << QStringList{"Harbour", "Intro", "Outro"} << qsizetype{0};
    QTest::newRow("an entry over the Current Entry down")
        << qsizetype{0} << qsizetype{2} << QStringList{"Harbour", "Outro", "Intro"} << qsizetype{0};
    QTest::newRow("an entry over the Current Entry up")
        << qsizetype{2} << qsizetype{0} << QStringList{"Outro", "Intro", "Harbour"} << qsizetype{2};
}

void QueueShould::move_entries_without_interrupting_playback()
{
    QFETCH(qsizetype, from);
    QFETCH(qsizetype, to);
    QFETCH(QStringList, expectedTitles);
    QFETCH(qsizetype, expectedCurrentIndex);
    mQueue->replace(album(), 1);
    report(uriOf(QStringLiteral("Harbour")), QStringLiteral("0:03:00"), QStringLiteral("0:01:42"));
    mDevice->reset();
    auto currentEntryChangedSpy = QSignalSpy{mQueue.get(), &Queue::currentEntryChanged};

    mQueue->move(from, to);

    auto expectedEntries = Items{};
    for (auto const& title : expectedTitles) {
        expectedEntries.append(playable(title));
    }
    QCOMPARE(mQueue->entries(), expectedEntries);
    QCOMPARE(mQueue->currentIndex(), std::optional{expectedCurrentIndex});
    QCOMPARE(currentEntryChangedSpy.size(), 0);
    QCOMPARE(mQueue->lastKnownPosition(), std::chrono::milliseconds{102'000});
    QCOMPARE(mQueue->state(), Queue::State::Running);
    QCOMPARE(mDevice->isSetAvTransportUriCalled(), false);
    QCOMPARE(mDevice->isStopCalled(), false);
}

void QueueShould::notify_about_a_move()
{
    mQueue->replace(album(), 1);
    auto aboutToBeMovedSpy = QSignalSpy{mQueue.get(), &Queue::entryAboutToBeMoved};
    auto movedSpy = QSignalSpy{mQueue.get(), &Queue::entryMoved};

    mQueue->move(0, 2);

    QCOMPARE(aboutToBeMovedSpy.size(), 1);
    QCOMPARE(aboutToBeMovedSpy.at(0).at(0).value<qsizetype>(), 0);
    QCOMPARE(aboutToBeMovedSpy.at(0).at(1).value<qsizetype>(), 2);
    QCOMPARE(movedSpy.size(), 1);
}

void QueueShould::ignore_an_invalid_move()
{
    mQueue->replace(album(), 1);
    auto aboutToBeMovedSpy = QSignalSpy{mQueue.get(), &Queue::entryAboutToBeMoved};

    mQueue->move(0, 0);
    mQueue->move(0, 3);
    mQueue->move(-1, 1);

    QCOMPARE(mQueue->entries(), album());
    QCOMPARE(aboutToBeMovedSpy.size(), 0);
}

void QueueShould::empty_the_queue_make_it_idle_and_stop_the_renderer_on_clear()
{
    mQueue->replace(album(), 1);
    report(uriOf(QStringLiteral("Harbour")), QStringLiteral("0:03:00"), QStringLiteral("0:01:42"));
    mDevice->reset();
    auto replacedSpy = QSignalSpy{mQueue.get(), &Queue::entriesReplaced};
    auto currentEntryChangedSpy = QSignalSpy{mQueue.get(), &Queue::currentEntryChanged};

    mQueue->clear();

    QCOMPARE(mQueue->entries(), Items{});
    QCOMPARE(mQueue->currentIndex(), std::nullopt);
    QCOMPARE(mQueue->lastKnownPosition(), std::chrono::milliseconds{0});
    QCOMPARE(mQueue->state(), Queue::State::Idle);
    QCOMPARE(replacedSpy.size(), 1);
    QCOMPARE(currentEntryChangedSpy.size(), 1);
    QCOMPARE(mDevice->isStopCalled(), true);
}

void QueueShould::not_stop_another_controller_on_clear()
{
    mQueue->replace(album(), 1);
    // Another controller took over the Renderer.
    report(QStringLiteral("http://radio.example/stream.mp3"), QStringLiteral("0:00:00"), QStringLiteral("0:00:10"));
    mDevice->reset();

    mQueue->clear();

    QCOMPARE(mQueue->entries(), Items{});
    QCOMPARE(mQueue->state(), Queue::State::Idle);
    QCOMPARE(mDevice->isStopCalled(), false);
}

void QueueShould::restore_a_snapshot_without_interrupting_the_current_entry()
{
    mQueue->replace(album(), 1);
    report(uriOf(QStringLiteral("Harbour")), QStringLiteral("0:03:00"), QStringLiteral("0:01:42"));
    auto const snapshot = mQueue->snapshot();
    mQueue->remove(0);
    mDevice->reset();
    auto replacedSpy = QSignalSpy{mQueue.get(), &Queue::entriesReplaced};

    mQueue->restore(snapshot);

    QCOMPARE(mQueue->entries(), album());
    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{1});
    QCOMPARE(mQueue->lastKnownPosition(), std::chrono::milliseconds{102'000});
    QCOMPARE(mQueue->state(), Queue::State::Running);
    QCOMPARE(replacedSpy.size(), 1);
    QCOMPARE(mDevice->isSetAvTransportUriCalled(), false);
}

void QueueShould::play_the_restored_current_entry_again_when_it_played_before_data()
{
    QTest::addColumn<bool>("clear");

    QTest::newRow("after removing the Current Entry") << false;
    QTest::newRow("after a clear") << true;
}

void QueueShould::play_the_restored_current_entry_again_when_it_played_before()
{
    QFETCH(bool, clear);
    mQueue->replace(album(), 1);
    report(uriOf(QStringLiteral("Harbour")), QStringLiteral("0:03:00"), QStringLiteral("0:01:42"));
    auto const snapshot = mQueue->snapshot();
    clear ? mQueue->clear() : mQueue->remove(1);
    mDevice->setDeviceState(MediaDevice::State::Stopped);
    mDevice->reset();

    mQueue->restore(snapshot);
    Q_EMIT mDevice->avTransportUriCall()->finished();

    QCOMPARE(mQueue->entries(), album());
    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{1});
    QCOMPARE(mDevice->avTransportUriData().uri, uriOf(QStringLiteral("Harbour")));
    QCOMPARE(mDevice->isPlayCalled(), true);
    QCOMPARE(mQueue->state(), Queue::State::Running);
}

void QueueShould::restore_an_idle_queue_without_playing()
{
    makeIdle(1);
    auto const snapshot = mQueue->snapshot();
    mQueue->clear();
    auto currentEntryChangedSpy = QSignalSpy{mQueue.get(), &Queue::currentEntryChanged};

    mQueue->restore(snapshot);

    QCOMPARE(mQueue->entries(), album());
    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{1});
    QCOMPARE(currentEntryChangedSpy.size(), 1);
    QCOMPARE(mQueue->state(), Queue::State::Idle);
    QCOMPARE(mDevice->isSetAvTransportUriCalled(), false);
}

void QueueShould::hand_over_a_running_queue_to_the_new_active_renderer_data()
{
    QTest::addColumn<bool>("seekSupported");

    QTest::newRow("with seek support") << true;
    QTest::newRow("without seek support") << false;
}

void QueueShould::hand_over_a_running_queue_to_the_new_active_renderer()
{
    QFETCH(bool, seekSupported);
    mQueue->replace(album(), 1);
    reportPlayedFor(uriOf(QStringLiteral("Harbour")), std::chrono::seconds{80});

    handOverToBathroom(seekSupported, QStringLiteral("0:03:00"), QStringLiteral("0:00:00"));

    QCOMPARE(mDevice->isStopCalled(), true);
    QCOMPARE(mBathroomDevice->avTransportUriData().uri, uriOf(QStringLiteral("Harbour")));
    QCOMPARE(mBathroomDevice->isPlayCalled(), true);
    auto const expectedSeek = SeekData{.instanceId = 0, .mode = MediaDevice::SeekMode::RelTime, .target = "0:01:20"};
    QCOMPARE(mBathroomDevice->seekData(), seekSupported ? std::optional{expectedSeek} : std::nullopt);
    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{1});
    QCOMPARE(mQueue->state(), Queue::State::Running);
}

void QueueShould::hand_over_a_running_queue_after_another_controller_took_over()
{
    mQueue->replace(album(), 1);
    reportPlayedFor(uriOf(QStringLiteral("Harbour")), std::chrono::seconds{80});
    report(QStringLiteral("http://radio.example/stream.mp3"), QStringLiteral("0:00:00"), QStringLiteral("0:00:10"));
    mDevice->reset();

    handOverToBathroom(true, QStringLiteral("0:03:00"), QStringLiteral("0:00:00"));

    auto const expectedSeek = SeekData{.instanceId = 0, .mode = MediaDevice::SeekMode::RelTime, .target = "0:01:20"};
    QCOMPARE(mDevice->isStopCalled(), true);
    QCOMPARE(mBathroomDevice->avTransportUriData().uri, uriOf(QStringLiteral("Harbour")));
    QCOMPARE(mBathroomDevice->seekData(), std::optional{expectedSeek});
    QCOMPARE(mQueue->state(), Queue::State::Running);
}

void QueueShould::play_a_stream_from_its_start_on_a_hand_over()
{
    mQueue->replace(album(), 1);
    reportPlayedFor(uriOf(QStringLiteral("Harbour")), std::chrono::seconds{80});

    // Without a duration the new Renderer can't seek, the position is recorded right away.
    handOverToBathroom(true, QStringLiteral("0:00:00"), QStringLiteral("0:00:05"));

    QCOMPARE(mBathroomDevice->seekData(), std::nullopt);
    QCOMPARE(mQueue->lastKnownPosition(), std::chrono::milliseconds{5'000});
}

void QueueShould::stop_the_previous_renderer_while_it_loads_the_current_entry_on_a_hand_over()
{
    mQueue->replace(album(), 1);

    mQueue->setActiveRenderer(createBathroom());

    QCOMPARE(mDevice->isStopCalled(), true);
}

void QueueShould::not_stop_a_renderer_the_queue_was_still_handed_over_to()
{
    mQueue->replace(album(), 1);
    auto const bathroom = createBathroom();
    auto* const bathroomDevice = mBathroomDevice;
    mQueue->setActiveRenderer(bathroom);
    // Another controller plays on the Bathroom before the Hand Over to it is done.
    bathroomDevice->setDeviceState(MediaDevice::State::Playing);

    mQueue->setActiveRenderer(mRenderer);

    QCOMPARE(bathroomDevice->isStopCalled(), false);
    QCOMPARE(bathroomDevice->isSetAvTransportUriCalled(), false);
}

void QueueShould::become_idle_when_the_new_renderer_fails_to_initialize_on_a_hand_over()
{
    mQueue->replace(album(), 1);
    mQueue->setActiveRenderer(createBathroom());

    mBathroomDevice->protocolInfoCall()->setErrorState(true);
    Q_EMIT mBathroomDevice->protocolInfoCall()->finished();

    QCOMPARE(mBathroomDevice->isSetAvTransportUriCalled(), false);
    QCOMPARE(mQueue->state(), Queue::State::Idle);
    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{1});
}

void QueueShould::not_hand_over_when_paused_during_a_pending_hand_over()
{
    mQueue->replace(album(), 1);
    mQueue->setActiveRenderer(createBathroom());
    mBathroomDevice->setDeviceState(MediaDevice::State::Playing);

    mQueue->togglePlayback();
    Q_EMIT mBathroomDevice->protocolInfoCall()->finished();

    QCOMPARE(mBathroomDevice->isStopCalled(), true);
    QCOMPARE(mBathroomDevice->isSetAvTransportUriCalled(), false);
    QCOMPARE(mQueue->state(), Queue::State::Idle);
}

void QueueShould::not_hand_over_an_idle_queue()
{
    makeIdle(1);
    mDevice->setDeviceState(MediaDevice::State::Playing);
    auto const newRenderer = createBathroom();

    mQueue->setActiveRenderer(newRenderer);
    Q_EMIT mBathroomDevice->protocolInfoCall()->finished();

    QCOMPARE(mDevice->isStopCalled(), false);
    QCOMPARE(mBathroomDevice->isSetAvTransportUriCalled(), false);
    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{1});
    QCOMPARE(mQueue->state(), Queue::State::Idle);
}

void QueueShould::become_idle_and_keep_the_current_entry_when_the_active_renderer_goes_offline()
{
    mQueue->replace(album(), 1);
    reportPlayedFor(uriOf(QStringLiteral("Harbour")), std::chrono::seconds{80});
    auto stateChangedSpy = QSignalSpy{mQueue.get(), &Queue::stateChanged};

    mRenderer->goOffline();

    QCOMPARE(stateChangedSpy.size(), 1);
    QCOMPARE(mQueue->state(), Queue::State::Idle);
    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{1});
    QCOMPARE(mQueue->lastKnownPosition(), std::chrono::milliseconds{80'000});

    // An Idle Queue isn't handed over to the next Active Renderer.
    auto const newRenderer = createBathroom();
    mQueue->setActiveRenderer(newRenderer);
    Q_EMIT mBathroomDevice->protocolInfoCall()->finished();
    QCOMPARE(mBathroomDevice->isSetAvTransportUriCalled(), false);
}

void QueueShould::become_idle_when_the_active_renderer_is_unset()
{
    mQueue->replace(album(), 1);
    reportPlayedFor(uriOf(QStringLiteral("Harbour")), std::chrono::seconds{80});

    mQueue->setActiveRenderer(nullptr);

    QCOMPARE(mQueue->state(), Queue::State::Idle);
    QCOMPARE(mQueue->currentIndex(), std::optional<qsizetype>{1});
    QCOMPARE(mQueue->lastKnownPosition(), std::chrono::milliseconds{80'000});
}

void QueueShould::pause_the_renderer_and_become_idle_on_toggle_while_it_plays_data()
{
    QTest::addColumn<QString>("currentTrackUri");

    QTest::newRow("Current Entry") << uriOf(QStringLiteral("Harbour"));
    QTest::newRow("another controller") << QStringLiteral("http://radio.example/stream.mp3");
}

void QueueShould::pause_the_renderer_and_become_idle_on_toggle_while_it_plays()
{
    QFETCH(QString, currentTrackUri);
    mQueue->replace(album(), 1);
    report(currentTrackUri, QStringLiteral("0:03:00"), QStringLiteral("0:00:10"));
    mDevice->reset();
    mDevice->setPauseEnabled(true);

    mQueue->togglePlayback();

    QCOMPARE(mDevice->isPauseCalled(), true);
    QCOMPARE(mQueue->state(), Queue::State::Idle);
}

void QueueShould::resume_the_current_entry_and_run_on_toggle()
{
    mQueue->replace(album(), 1);
    reportPlayedFor(uriOf(QStringLiteral("Harbour")), std::chrono::seconds{80});
    mQueue->togglePlayback();
    Q_EMIT mDevice->stopCall()->finished();
    mDevice->setDeviceState(MediaDevice::State::Stopped);
    mDevice->reset();

    mQueue->togglePlayback();

    QCOMPARE(mDevice->isPlayCalled(), true);
    QCOMPARE(mDevice->isSetAvTransportUriCalled(), false);
    QCOMPARE(mQueue->state(), Queue::State::Running);
}

void QueueShould::continue_the_current_entry_at_the_last_known_position_on_toggle_of_an_idle_queue()
{
    mDevice->setRelTimeSeekEnabled(true);
    mQueue->replace(album(), 1);
    reportPlayedFor(uriOf(QStringLiteral("Harbour")), std::chrono::seconds{80});
    report(QStringLiteral("http://other.example/song.mp3"), QStringLiteral("0:04:00"), QStringLiteral("0:00:10"));
    mQueue->togglePlayback();
    Q_EMIT mDevice->stopCall()->finished();
    mDevice->setDeviceState(MediaDevice::State::Stopped);
    mDevice->reset();

    mQueue->togglePlayback();
    Q_EMIT mDevice->avTransportUriCall()->finished();
    report(uriOf(QStringLiteral("Harbour")), QStringLiteral("0:03:00"), QStringLiteral("0:00:00"));

    auto const expectedSeek = SeekData{.instanceId = 0, .mode = MediaDevice::SeekMode::RelTime, .target = "0:01:20"};
    QCOMPARE(mDevice->avTransportUriData().uri, uriOf(QStringLiteral("Harbour")));
    QCOMPARE(mDevice->isPlayCalled(), true);
    QCOMPARE(mDevice->seekData(), std::optional{expectedSeek});
    QCOMPARE(mQueue->state(), Queue::State::Running);
}

void QueueShould::resume_the_renderer_on_toggle_of_an_empty_queue()
{
    mDevice->setDeviceState(MediaDevice::State::PausedPlayback);

    mQueue->togglePlayback();

    QCOMPARE(mDevice->isPlayCalled(), true);
    QCOMPARE(mQueue->state(), Queue::State::Idle);
}

void QueueShould::ignore_toggling_while_a_playback_control_is_pending_or_transitioning()
{
    mQueue->replace(album(), 1);
    report(uriOf(QStringLiteral("Harbour")), QStringLiteral("0:03:00"), QStringLiteral("0:00:10"));
    mDevice->reset();
    mQueue->togglePlayback();
    mDevice->setDeviceState(MediaDevice::State::Stopped);

    mQueue->togglePlayback();
    QCOMPARE(mDevice->isPlayCalled(), false);

    Q_EMIT mDevice->stopCall()->finished();
    mDevice->setDeviceState(MediaDevice::State::Transitioning);
    mQueue->togglePlayback();
    QCOMPARE(mDevice->isPlayCalled(), false);
}

} // namespace Multimedia

QTEST_MAIN(Multimedia::QueueShould)
