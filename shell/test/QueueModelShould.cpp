// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "QueueModelShould.hpp"
#include "Descriptions.hpp"
#include "InMemoryRendererStore.hpp"
#include "PositionInfoResponse.hpp"
#include "RendererProvider.hpp"
#include "TestSource.hpp"
#include <QAbstractItemModelTester>
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

Item playable(QString const& title, QString const& artist = {}, QString const& artworkUrl = {})
{
    return ItemBuilder{}
        .withItemType(ItemType::Playable)
        .withMainText(title)
        .withSecondaryText(artist)
        .withArtworkUrl(artworkUrl)
        .withPlayUrl(QStringLiteral("http://192.168.0.3:8200/MediaItems/%1.mp3").arg(title))
        .withSupportedTypes({Protocol::create(QStringLiteral("http-get:*:audio/mpeg")).value_or(Protocol{})})
        .build();
}

Items album()
{
    return {playable(QStringLiteral("Intro"), QStringLiteral("The Quiet Ferries"), QStringLiteral("http://art/1.jpg")),
            playable(QStringLiteral("Harbour"))};
}

Item song(QString const& title, QString const& album, std::optional<std::chrono::milliseconds> duration)
{
    return ItemBuilder{}
        .withItemType(ItemType::Playable)
        .withMainText(title)
        .withAlbum(album)
        .withDuration(duration)
        .withPlayUrl(QStringLiteral("http://192.168.0.3:8200/MediaItems/%1.mp3").arg(title))
        .build();
}

QVariant dataOf(QueueModel const& model, int row, QueueModel::DisplayRole role)
{
    return model.data(model.index(row), static_cast<int>(role));
}

QStringList titlesOf(QueueModel const& model)
{
    auto titles = QStringList{};
    for (auto row = 0; row < model.rowCount(); ++row) {
        titles.append(dataOf(model, row, QueueModel::DisplayRole::Title).toString());
    }
    return titles;
}

Items threeSongs()
{
    return {playable(QStringLiteral("Intro")), playable(QStringLiteral("Harbour")), playable(QStringLiteral("Outro"))};
}

Item artist()
{
    return ItemBuilder{}
        .withItemType(ItemType::Container)
        .withMainText(QStringLiteral("The Quiet Ferries"))
        .withPath(QStringLiteral("artist"))
        .build();
}

/**
 * Gives a Source whose artist Container holds the Playables "Bonus" and "Hidden".
 */
std::shared_ptr<TestHelper::TestSource> createSource()
{
    auto source = std::make_shared<TestHelper::TestSource>(QStringLiteral("NAS"), QString{});
    source->setItems(QStringLiteral("artist"), {playable(QStringLiteral("Bonus")), playable(QStringLiteral("Hidden"))});
    return source;
}
} // namespace

QueueModelShould::~QueueModelShould() = default;

void QueueModelShould::init()
{
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
    mRendererModel = std::make_unique<MediaRendererModel>(std::move(rProvider));
    mModel = std::make_unique<QueueModel>(*mRendererModel);

    Q_EMIT mServiceProvider->serviceConnected(kitchenUsn);
    Q_EMIT mServiceProvider->serviceConnected(bathroomUsn);
}

void QueueModelShould::cleanup()
{
    mModel.reset();
    mRendererModel.reset();
}

void QueueModelShould::activate(QString const& name)
{
    for (auto row = 0; row < mRendererModel->rowCount(); ++row) {
        auto const index = mRendererModel->index(row);
        if (mRendererModel->data(index, static_cast<int>(MediaRendererModel::DisplayRole::Name)).toString() == name) {
            mRendererModel->activateRenderer(index);
            Q_EMIT device(name)->protocolInfoCall()->finished();
            return;
        }
    }
    QFAIL("No Renderer with the name found.");
}

MediaRendererDouble* QueueModelShould::device(QString const& name) const noexcept
{
    return mRendererFactory->renderer(name);
}

void QueueModelShould::be_empty_and_not_running_at_start()
{
    QCOMPARE(mModel->rowCount(), 0);
    QCOMPARE(mModel->property("running").toBool(), false);
}

void QueueModelShould::give_the_role_names()
{
    auto const expectedRoles = QHash<int, QByteArray>{
        {static_cast<int>(QueueModel::DisplayRole::Title), QByteArray{"title"}},
        {static_cast<int>(QueueModel::DisplayRole::Artist), QByteArray{"artist"}},
        {static_cast<int>(QueueModel::DisplayRole::ArtworkUrl), QByteArray{"artworkUrl"}},
        {static_cast<int>(QueueModel::DisplayRole::Current), QByteArray{"current"}},
        {static_cast<int>(QueueModel::DisplayRole::Album), QByteArray{"album"}},
        {static_cast<int>(QueueModel::DisplayRole::Duration), QByteArray{"duration"}},
        {static_cast<int>(QueueModel::DisplayRole::HasDuration), QByteArray{"hasDuration"}},
    };

    QCOMPARE(mModel->roleNames(), expectedRoles);
}

void QueueModelShould::give_the_title_artist_and_artwork_of_the_entries_and_mark_the_current_entry()
{
    auto tester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};

    mModel->replace(album(), 1);

    QCOMPARE(mModel->rowCount(), 2);
    QCOMPARE(dataOf(*mModel, 0, QueueModel::DisplayRole::Title).toString(), QStringLiteral("Intro"));
    QCOMPARE(dataOf(*mModel, 0, QueueModel::DisplayRole::Artist).toString(), QStringLiteral("The Quiet Ferries"));
    QCOMPARE(dataOf(*mModel, 0, QueueModel::DisplayRole::ArtworkUrl).toString(), QStringLiteral("http://art/1.jpg"));
    QCOMPARE(dataOf(*mModel, 0, QueueModel::DisplayRole::Current).toBool(), false);
    QCOMPARE(dataOf(*mModel, 1, QueueModel::DisplayRole::Title).toString(), QStringLiteral("Harbour"));
    QCOMPARE(dataOf(*mModel, 1, QueueModel::DisplayRole::Current).toBool(), true);
}

void QueueModelShould::reset_when_the_queue_is_replaced()
{
    auto tester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};
    mModel->replace(album(), 0);
    auto modelResetSpy = QSignalSpy{mModel.get(), &QueueModel::modelReset};

    mModel->replace({playable(QStringLiteral("Outro"))}, 0);

    QCOMPARE(modelResetSpy.size(), 1);
    QCOMPARE(mModel->rowCount(), 1);
}

void QueueModelShould::play_on_the_active_renderer_and_run_after_a_replace()
{
    activate(QStringLiteral("Kitchen"));
    auto runningChangedSpy = QSignalSpy{mModel.get(), &QueueModel::runningChanged};

    mModel->replace(album(), 1);

    QCOMPARE(device(QStringLiteral("Kitchen"))->avTransportUriData().uri,
             playable(QStringLiteral("Harbour")).playUrl());
    QCOMPARE(mModel->property("running").toBool(), true);
    QCOMPARE(runningChangedSpy.size(), 1);
}

void QueueModelShould::follow_a_switch_of_the_active_renderer()
{
    activate(QStringLiteral("Kitchen"));
    activate(QStringLiteral("Bathroom"));

    mModel->replace(album(), 0);

    QCOMPARE(device(QStringLiteral("Bathroom"))->avTransportUriData().uri, playable(QStringLiteral("Intro")).playUrl());
    QCOMPARE(device(QStringLiteral("Kitchen"))->isSetAvTransportUriCalled(), false);
}

void QueueModelShould::only_hold_the_entries_without_an_active_renderer()
{
    mModel->replace(album(), 0);

    QCOMPARE(mModel->rowCount(), 2);
    QCOMPARE(mModel->property("running").toBool(), false);
}

void QueueModelShould::mark_the_next_entry_as_current_when_the_queue_advances()
{
    auto tester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};
    activate(QStringLiteral("Kitchen"));
    mModel->replace(album(), 0);
    auto dataChangedSpy = QSignalSpy{mModel.get(), &QueueModel::dataChanged};
    auto* const kitchen = device(QStringLiteral("Kitchen"));
    auto const introUri = playable(QStringLiteral("Intro")).playUrl();

    kitchen->setCurrentTrack(introUri, QString{});
    kitchen->setDeviceState(MediaDevice::State::Playing);
    kitchen->finishPositionInfoCall(positionInfoResponse(introUri, QString{}, "0:03:00", "0:02:59"));
    kitchen->setDeviceState(MediaDevice::State::Stopped);

    QCOMPARE(dataOf(*mModel, 0, QueueModel::DisplayRole::Current).toBool(), false);
    QCOMPARE(dataOf(*mModel, 1, QueueModel::DisplayRole::Current).toBool(), true);
    QCOMPARE(dataChangedSpy.size(), 2);
}

void QueueModelShould::give_the_album_and_the_duration_of_the_entries()
{
    using namespace std::chrono_literals;
    mModel->replace({song(QStringLiteral("Intro"), QStringLiteral("Low Tide Sessions"), 4min + 31s),
                     song(QStringLiteral("Stream"), QString{}, std::nullopt)},
                    0);

    QCOMPARE(dataOf(*mModel, 0, QueueModel::DisplayRole::Album).toString(), QStringLiteral("Low Tide Sessions"));
    QCOMPARE(dataOf(*mModel, 0, QueueModel::DisplayRole::Duration).toDouble(), 271'000.0);
    QCOMPARE(dataOf(*mModel, 0, QueueModel::DisplayRole::HasDuration).toBool(), true);
    QCOMPARE(dataOf(*mModel, 1, QueueModel::DisplayRole::Album).toString(), QString{});
    QCOMPARE(dataOf(*mModel, 1, QueueModel::DisplayRole::Duration).toDouble(), 0.0);
    QCOMPARE(dataOf(*mModel, 1, QueueModel::DisplayRole::HasDuration).toBool(), false);
}

void QueueModelShould::fall_back_to_the_file_name_as_title()
{
    auto const untitled = ItemBuilder{}
                              .withItemType(ItemType::Playable)
                              .withPlayUrl(QStringLiteral("http://192.168.0.3/Harbour%20Lights.flac"))
                              .build();

    mModel->replace({untitled}, 0);

    QCOMPARE(dataOf(*mModel, 0, QueueModel::DisplayRole::Title).toString(), QStringLiteral("Harbour Lights"));
}

void QueueModelShould::summarize_the_entry_count_and_the_total_duration_data()
{
    using namespace std::chrono_literals;
    QTest::addColumn<Items>("entries");
    QTest::addColumn<int>("expectedEntryCount");
    QTest::addColumn<double>("expectedTotalDuration");
    QTest::addColumn<bool>("expectedHasTotalDuration");
    QTest::addColumn<bool>("expectedTotalDurationPartial");

    QTest::newRow("empty") << Items{} << 0 << 0.0 << false << false;
    QTest::newRow("all durations known") << Items{song(QStringLiteral("A"), QString{}, 3min),
                                                  song(QStringLiteral("B"), QString{}, 4min + 30s)}
                                         << 2 << 450'000.0 << true << false;
    QTest::newRow("some durations unknown")
        << Items{song(QStringLiteral("A"), QString{}, 3min), song(QStringLiteral("B"), QString{}, std::nullopt)} << 2
        << 180'000.0 << true << true;
    QTest::newRow("no duration known") << Items{song(QStringLiteral("A"), QString{}, std::nullopt)} << 1 << 0.0 << false
                                       << true;
}

void QueueModelShould::summarize_the_entry_count_and_the_total_duration()
{
    QFETCH(Items, entries);
    QFETCH(int, expectedEntryCount);
    QFETCH(double, expectedTotalDuration);
    QFETCH(bool, expectedHasTotalDuration);
    QFETCH(bool, expectedTotalDurationPartial);

    mModel->replace(entries, 0);

    QCOMPARE(mModel->property("entryCount").toInt(), expectedEntryCount);
    QCOMPARE(mModel->property("totalDuration").toDouble(), expectedTotalDuration);
    QCOMPARE(mModel->property("hasTotalDuration").toBool(), expectedHasTotalDuration);
    QCOMPARE(mModel->property("totalDurationPartial").toBool(), expectedTotalDurationPartial);
}

void QueueModelShould::notify_about_a_changed_summary()
{
    auto summaryChangedSpy = QSignalSpy{mModel.get(), &QueueModel::summaryChanged};

    mModel->replace(album(), 0);

    QCOMPARE(summaryChangedSpy.size(), 1);
}

void QueueModelShould::play_the_entry_at_a_row()
{
    auto tester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};
    activate(QStringLiteral("Kitchen"));
    mModel->replace(album(), 0);
    auto* const kitchen = device(QStringLiteral("Kitchen"));
    kitchen->reset();

    mModel->play(1);

    QCOMPARE(kitchen->avTransportUriData().uri, playable(QStringLiteral("Harbour")).playUrl());
    QCOMPARE(dataOf(*mModel, 1, QueueModel::DisplayRole::Current).toBool(), true);
    QCOMPARE(mModel->property("running").toBool(), true);
}

void QueueModelShould::only_make_the_entry_at_a_row_current_without_an_active_renderer()
{
    mModel->replace(album(), 0);

    mModel->play(1);

    QCOMPARE(dataOf(*mModel, 1, QueueModel::DisplayRole::Current).toBool(), true);
    QCOMPARE(mModel->property("running").toBool(), false);
}

void QueueModelShould::tell_whether_the_active_renderer_plays_the_current_entry()
{
    activate(QStringLiteral("Kitchen"));
    mModel->replace(album(), 0);
    auto currentEntryPlayingChangedSpy = QSignalSpy{mModel.get(), &QueueModel::currentEntryPlayingChanged};
    auto* const kitchen = device(QStringLiteral("Kitchen"));

    kitchen->setCurrentTrack(playable(QStringLiteral("Intro")).playUrl(), QString{});
    kitchen->setDeviceState(MediaDevice::State::Playing);

    QCOMPARE(mModel->property("currentEntryPlaying").toBool(), true);
    QCOMPARE(currentEntryPlayingChangedSpy.size(), 1);
}

void QueueModelShould::append_a_playable_and_play_it()
{
    auto tester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};
    activate(QStringLiteral("Kitchen"));
    mModel->replace(album(), 0);
    auto rowsInsertedSpy = QSignalSpy{mModel.get(), &QueueModel::rowsInserted};
    auto summaryChangedSpy = QSignalSpy{mModel.get(), &QueueModel::summaryChanged};
    auto* const kitchen = device(QStringLiteral("Kitchen"));
    kitchen->reset();

    mModel->appendAndPlay(playable(QStringLiteral("Bonus")));

    QCOMPARE(mModel->rowCount(), 3);
    QCOMPARE(rowsInsertedSpy.size(), 1);
    QCOMPARE(summaryChangedSpy.size(), 1);
    QCOMPARE(dataOf(*mModel, 2, QueueModel::DisplayRole::Title).toString(), QStringLiteral("Bonus"));
    QCOMPARE(dataOf(*mModel, 0, QueueModel::DisplayRole::Current).toBool(), false);
    QCOMPARE(dataOf(*mModel, 2, QueueModel::DisplayRole::Current).toBool(), true);
    QCOMPARE(kitchen->avTransportUriData().uri, playable(QStringLiteral("Bonus")).playUrl());
    QCOMPARE(mModel->property("running").toBool(), true);
}

void QueueModelShould::only_append_a_playable_and_make_it_current_without_an_active_renderer()
{
    auto tester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};

    mModel->appendAndPlay(playable(QStringLiteral("Bonus")));

    QCOMPARE(mModel->rowCount(), 1);
    QCOMPARE(dataOf(*mModel, 0, QueueModel::DisplayRole::Current).toBool(), true);
    QCOMPARE(mModel->property("running").toBool(), false);
}

void QueueModelShould::tell_whether_previous_and_next_are_available()
{
    QCOMPARE(mModel->property("hasPrevious").toBool(), false);
    QCOMPARE(mModel->property("hasNext").toBool(), false);

    mModel->replace(album(), 0);
    QCOMPARE(mModel->property("hasPrevious").toBool(), true);
    QCOMPARE(mModel->property("hasNext").toBool(), true);

    mModel->play(1);
    QCOMPARE(mModel->property("hasPrevious").toBool(), true);
    QCOMPARE(mModel->property("hasNext").toBool(), false);
}

void QueueModelShould::notify_when_the_availability_of_previous_and_next_changed()
{
    auto stepAvailabilityChangedSpy = QSignalSpy{mModel.get(), &QueueModel::stepAvailabilityChanged};

    mModel->replace(album(), 0);
    QCOMPARE(stepAvailabilityChangedSpy.isEmpty(), false);
    stepAvailabilityChangedSpy.clear();

    mModel->play(1);
    QCOMPARE(stepAvailabilityChangedSpy.isEmpty(), false);
}

void QueueModelShould::step_to_the_next_and_the_previous_entry()
{
    auto tester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};
    activate(QStringLiteral("Kitchen"));
    mModel->replace(album(), 0);
    auto* const kitchen = device(QStringLiteral("Kitchen"));
    kitchen->reset();

    mModel->next();

    QCOMPARE(kitchen->avTransportUriData().uri, playable(QStringLiteral("Harbour")).playUrl());
    QCOMPARE(dataOf(*mModel, 1, QueueModel::DisplayRole::Current).toBool(), true);

    mModel->previous();

    QCOMPARE(kitchen->avTransportUriData().uri, playable(QStringLiteral("Intro")).playUrl());
    QCOMPARE(dataOf(*mModel, 0, QueueModel::DisplayRole::Current).toBool(), true);
    QCOMPARE(mModel->property("running").toBool(), true);
}

void QueueModelShould::remove_the_row_of_an_entry()
{
    auto tester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};
    mModel->replace(threeSongs(), 1);
    auto rowsRemovedSpy = QSignalSpy{mModel.get(), &QueueModel::rowsRemoved};
    auto summaryChangedSpy = QSignalSpy{mModel.get(), &QueueModel::summaryChanged};
    auto stepAvailabilityChangedSpy = QSignalSpy{mModel.get(), &QueueModel::stepAvailabilityChanged};

    mModel->remove(2);

    QCOMPARE(rowsRemovedSpy.size(), 1);
    QCOMPARE(rowsRemovedSpy.at(0).at(1).toInt(), 2);
    QCOMPARE(summaryChangedSpy.size(), 1);
    QCOMPARE(stepAvailabilityChangedSpy.isEmpty(), false);
    QCOMPARE(titlesOf(*mModel), (QStringList{"Intro", "Harbour"}));
    QCOMPARE(dataOf(*mModel, 1, QueueModel::DisplayRole::Current).toBool(), true);
    QCOMPARE(mModel->property("hasNext").toBool(), false);

    mModel->remove(0);

    QCOMPARE(titlesOf(*mModel), (QStringList{"Harbour"}));
    QCOMPARE(dataOf(*mModel, 0, QueueModel::DisplayRole::Current).toBool(), true);
}

void QueueModelShould::mark_the_next_entry_as_current_when_the_current_entry_is_removed()
{
    auto tester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};
    activate(QStringLiteral("Kitchen"));
    mModel->replace(threeSongs(), 1);
    auto* const kitchen = device(QStringLiteral("Kitchen"));
    kitchen->reset();

    mModel->remove(1);

    QCOMPARE(titlesOf(*mModel), (QStringList{"Intro", "Outro"}));
    QCOMPARE(dataOf(*mModel, 0, QueueModel::DisplayRole::Current).toBool(), false);
    QCOMPARE(dataOf(*mModel, 1, QueueModel::DisplayRole::Current).toBool(), true);
    QCOMPARE(kitchen->avTransportUriData().uri, playable(QStringLiteral("Outro")).playUrl());

    mModel->remove(1);

    QCOMPARE(titlesOf(*mModel), (QStringList{"Intro"}));
    QCOMPARE(dataOf(*mModel, 0, QueueModel::DisplayRole::Current).toBool(), false);
    QCOMPARE(mModel->property("running").toBool(), false);
    QCOMPARE(mModel->property("hasPrevious").toBool(), false);
}

void QueueModelShould::move_the_row_of_an_entry_data()
{
    QTest::addColumn<int>("from");
    QTest::addColumn<int>("to");
    QTest::addColumn<QStringList>("expectedTitles");
    QTest::addColumn<int>("expectedCurrentRow");

    QTest::newRow("down") << 0 << 2 << QStringList{"Harbour", "Outro", "Intro"} << 0;
    QTest::newRow("up") << 2 << 0 << QStringList{"Outro", "Intro", "Harbour"} << 2;
    QTest::newRow("the Current Entry") << 1 << 2 << QStringList{"Intro", "Outro", "Harbour"} << 2;
}

void QueueModelShould::move_the_row_of_an_entry()
{
    QFETCH(int, from);
    QFETCH(int, to);
    QFETCH(QStringList, expectedTitles);
    QFETCH(int, expectedCurrentRow);
    auto tester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};
    mModel->replace(threeSongs(), 1);
    auto rowsMovedSpy = QSignalSpy{mModel.get(), &QueueModel::rowsMoved};

    mModel->move(from, to);

    QCOMPARE(rowsMovedSpy.size(), 1);
    QCOMPARE(titlesOf(*mModel), expectedTitles);
    for (auto row = 0; row < mModel->rowCount(); ++row) {
        QCOMPARE(dataOf(*mModel, row, QueueModel::DisplayRole::Current).toBool(), row == expectedCurrentRow);
    }
    QCOMPARE(mModel->property("hasNext").toBool(), expectedCurrentRow < 2);
}

void QueueModelShould::clear_all_rows()
{
    auto tester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};
    activate(QStringLiteral("Kitchen"));
    mModel->replace(threeSongs(), 1);
    auto summaryChangedSpy = QSignalSpy{mModel.get(), &QueueModel::summaryChanged};

    mModel->clear();

    QCOMPARE(mModel->rowCount(), 0);
    QCOMPARE(mModel->property("entryCount").toInt(), 0);
    QCOMPARE(summaryChangedSpy.size(), 1);
    QCOMPARE(mModel->property("running").toBool(), false);
    QCOMPARE(mModel->property("hasPrevious").toBool(), false);
}

void QueueModelShould::undo_a_remove_or_a_clear_data()
{
    QTest::addColumn<bool>("clear");

    QTest::newRow("remove") << false;
    QTest::newRow("clear") << true;
}

void QueueModelShould::undo_a_remove_or_a_clear()
{
    QFETCH(bool, clear);
    auto tester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};
    mModel->replace(threeSongs(), 1);
    clear ? mModel->clear() : mModel->remove(1);

    mModel->undo();

    QCOMPARE(titlesOf(*mModel), (QStringList{"Intro", "Harbour", "Outro"}));
    QCOMPARE(dataOf(*mModel, 1, QueueModel::DisplayRole::Current).toBool(), true);
    QCOMPARE(mModel->property("entryCount").toInt(), 3);

    // A second undo has nothing to restore.
    mModel->remove(0);
    mModel->undo();
    mModel->undo();
    QCOMPARE(mModel->rowCount(), 3);
}

void QueueModelShould::ignore_undo_without_a_remove_or_a_clear()
{
    mModel->replace(threeSongs(), 1);
    auto modelResetSpy = QSignalSpy{mModel.get(), &QueueModel::modelReset};

    mModel->undo();

    QCOMPARE(modelResetSpy.size(), 0);
    QCOMPARE(mModel->rowCount(), 3);
}

void QueueModelShould::insert_the_rows_of_playables_played_next_or_added_to_the_queue()
{
    auto tester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};
    auto const source = createSource();
    activate(QStringLiteral("Kitchen"));
    mModel->replace(threeSongs(), 0);
    auto rowsInsertedSpy = QSignalSpy{mModel.get(), &QueueModel::rowsInserted};
    auto summaryChangedSpy = QSignalSpy{mModel.get(), &QueueModel::summaryChanged};

    mModel->playNext(source, playable(QStringLiteral("Bonus")));
    mModel->addToQueue(source, artist());

    QCOMPARE(titlesOf(*mModel),
             (QStringList{QStringLiteral("Intro"),
                          QStringLiteral("Bonus"),
                          QStringLiteral("Harbour"),
                          QStringLiteral("Outro"),
                          QStringLiteral("Bonus"),
                          QStringLiteral("Hidden")}));
    QCOMPARE(rowsInsertedSpy.size(), 2);
    QCOMPARE(summaryChangedSpy.size(), 2);
    QCOMPARE(dataOf(*mModel, 0, QueueModel::DisplayRole::Current).toBool(), true);
    QCOMPARE(mModel->hasNext(), true);
}

void QueueModelShould::make_the_first_added_playable_current_in_an_empty_queue()
{
    auto tester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};
    auto const source = createSource();
    auto stepAvailabilitySpy = QSignalSpy{mModel.get(), &QueueModel::stepAvailabilityChanged};

    mModel->addToQueue(source, artist());

    QCOMPARE(titlesOf(*mModel), (QStringList{QStringLiteral("Bonus"), QStringLiteral("Hidden")}));
    QCOMPARE(dataOf(*mModel, 0, QueueModel::DisplayRole::Current).toBool(), true);
    QCOMPARE(mModel->property("running").toBool(), false);
    QCOMPARE(mModel->hasNext(), true);
    QVERIFY(stepAvailabilitySpy.size() > 0);
}

void QueueModelShould::tell_while_a_container_is_collected()
{
    auto const source = createSource();
    source->setHoldPages(true);
    auto collectingChangedSpy = QSignalSpy{mModel.get(), &QueueModel::collectingChanged};

    mModel->playNext(source, artist());
    QCOMPARE(mModel->property("collecting").toBool(), true);
    QCOMPARE(mModel->property("collectedContainerTitle").toString(), QStringLiteral("The Quiet Ferries"));
    QCOMPARE(collectingChangedSpy.size(), 1);
    source->finishPendingPage();

    QCOMPARE(mModel->property("collecting").toBool(), false);
    QCOMPARE(mModel->property("collectedContainerTitle").toString(), QString{});
    QCOMPARE(collectingChangedSpy.size(), 2);
    QCOMPARE(mModel->rowCount(), 2);
}

void QueueModelShould::cancel_the_collection_of_a_container()
{
    auto const source = createSource();
    source->setHoldPages(true);
    mModel->addToQueue(source, artist());

    mModel->cancelCollection();

    QCOMPARE(mModel->property("collecting").toBool(), false);
    QCOMPARE(source->pendingPageCount(), 0);
    QCOMPARE(mModel->rowCount(), 0);
}

void QueueModelShould::report_a_failed_collection_with_the_title_of_the_container()
{
    auto const source = createSource();
    source->setHoldPages(true);
    auto collectionFailedSpy = QSignalSpy{mModel.get(), &QueueModel::collectionFailed};
    mModel->addToQueue(source, artist());

    source->failPendingPage();

    QCOMPARE(collectionFailedSpy.size(), 1);
    QCOMPARE(collectionFailedSpy.at(0).at(0).toString(), QStringLiteral("The Quiet Ferries"));
    QCOMPARE(mModel->rowCount(), 0);
}

void QueueModelShould::not_undo_after_playables_were_added()
{
    auto const source = createSource();
    mModel->replace(threeSongs(), 0);
    mModel->remove(2);

    mModel->addToQueue(source, playable(QStringLiteral("Bonus")));
    mModel->undo();

    QCOMPARE(titlesOf(*mModel),
             (QStringList{QStringLiteral("Intro"), QStringLiteral("Harbour"), QStringLiteral("Bonus")}));
}

} // namespace Shell

QTEST_MAIN(Shell::QueueModelShould)
