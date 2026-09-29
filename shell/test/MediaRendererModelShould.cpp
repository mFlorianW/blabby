// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MediaRendererModelShould.hpp"
#include "Descriptions.hpp"
#include "RendererProvider.hpp"
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
constexpr auto livingRoomUsn = QLatin1StringView{"uuid:livingroom"};

constexpr auto nameRole = static_cast<int>(MediaRendererModel::DisplayRole::Name);
constexpr auto playbackStateRole = static_cast<int>(MediaRendererModel::DisplayRole::PlaybackState);
constexpr auto activeRole = static_cast<int>(MediaRendererModel::DisplayRole::Active);
constexpr auto manufacturerRole = static_cast<int>(MediaRendererModel::DisplayRole::Manufacturer);
constexpr auto modelNameRole = static_cast<int>(MediaRendererModel::DisplayRole::ModelName);
constexpr auto addressRole = static_cast<int>(MediaRendererModel::DisplayRole::Address);
constexpr auto availabilityRole = static_cast<int>(MediaRendererModel::DisplayRole::Availability);

RememberedRenderer rememberedKitchen()
{
    return RememberedRenderer{.identity = kitchenUsn,
                              .name = QStringLiteral("Old Kitchen"),
                              .manufacturer = QStringLiteral("Denon"),
                              .modelName = QStringLiteral("HEOS 1"),
                              .address = QStringLiteral("192.168.1.10")};
}

RememberedRenderer rememberedAttic()
{
    return RememberedRenderer{.identity = QStringLiteral("uuid:attic"),
                              .name = QStringLiteral("Attic"),
                              .manufacturer = {},
                              .modelName = {},
                              .address = QStringLiteral("192.168.1.11")};
}
} // namespace

MediaRendererModelShould::~MediaRendererModelShould() = default;

void MediaRendererModelShould::init()
{
    mStore = std::make_shared<TestHelper::InMemoryRendererStore>();
    restart();
}

void MediaRendererModelShould::restart()
{
    mModel.reset();
    auto sProvider = std::make_unique<ServiceProviderDouble>();
    sProvider->addDeviceDescription(kitchenUsn,
                                    validRendererDeviceDescription(QStringLiteral("Kitchen"),
                                                                   QStringLiteral("Denon"),
                                                                   QStringLiteral("HEOS 1"),
                                                                   kitchenUsn,
                                                                   QStringLiteral("192.168.1.42")));
    sProvider->addDeviceDescription(
        bathroomUsn,
        validRendererDeviceDescription(QStringLiteral("Bathroom"), QString{}, QString{}, bathroomUsn, QString{}));
    sProvider->addDeviceDescription(
        livingRoomUsn,
        validRendererDeviceDescription(QStringLiteral("living room"), QString{}, QString{}, livingRoomUsn, QString{}));
    mServiceProvider = sProvider.get();
    auto rendererFactory = std::make_unique<MediaRendererDoubleFactory>();
    mRendererFactory = rendererFactory.get();
    auto rProvider = std::make_unique<RendererProvider>(mStore, std::move(sProvider), std::move(rendererFactory));
    mModel = std::make_unique<MediaRendererModel>(std::move(rProvider));
}

QStringList MediaRendererModelShould::rendererNames() const
{
    auto names = QStringList{};
    for (auto row = 0; row < mModel->rowCount(); ++row) {
        names.append(name(row));
    }
    return names;
}

QString MediaRendererModelShould::name(int row) const
{
    return mModel->data(mModel->index(row), nameRole).toString();
}

bool MediaRendererModelShould::isActive(int row) const
{
    return mModel->data(mModel->index(row), activeRole).toBool();
}

Renderer::Availability MediaRendererModelShould::availability(int row) const
{
    return static_cast<Renderer::Availability>(mModel->data(mModel->index(row), availabilityRole).toInt());
}

void MediaRendererModelShould::give_correct_display_roles_for_the_ui()
{
    auto const expRoles = QHash<int, QByteArray>{
        std::make_pair(nameRole, QByteArray{"name"}),
        std::make_pair(playbackStateRole, QByteArray{"playbackState"}),
        std::make_pair(activeRole, QByteArray{"active"}),
        std::make_pair(manufacturerRole, QByteArray{"manufacturer"}),
        std::make_pair(modelNameRole, QByteArray{"modelName"}),
        std::make_pair(addressRole, QByteArray{"address"}),
        std::make_pair(availabilityRole, QByteArray{"availability"}),
    };

    auto const roleNames = mModel->roleNames();

    QCOMPARE(roleNames, expRoles);
}

void MediaRendererModelShould::start_a_mediarenderer_discover_on_init()
{
    QCOMPARE(mServiceProvider->isSearchTriggered(), true);
}

void MediaRendererModelShould::increase_the_rowCount_on_new_connected_mediarenderer()
{
    auto modelTester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};

    Q_EMIT mServiceProvider->serviceConnected(kitchenUsn);
    auto const rowCount = mModel->rowCount();

    QCOMPARE(rowCount, 1);
}

void MediaRendererModelShould::keep_a_disconnected_renderer_listed_as_offline()
{
    auto modelTester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};
    Q_EMIT mServiceProvider->serviceConnected(kitchenUsn);
    auto dataChangedSpy = QSignalSpy{mModel.get(), &MediaRendererModel::dataChanged};

    Q_EMIT mServiceProvider->serviceDisconnected(kitchenUsn);

    QCOMPARE(mModel->rowCount(), 1);
    QCOMPARE(availability(0), Renderer::Availability::Offline);
    QCOMPARE(name(0), QStringLiteral("Kitchen"));
    QCOMPARE(mModel->data(mModel->index(0), addressRole).toString(), QStringLiteral("192.168.1.42"));
    auto const availabilityChanged = std::any_of(dataChangedSpy.cbegin(), dataChangedSpy.cend(), [](auto const& args) {
        return args.at(0).template value<QModelIndex>().row() == 0 and
               args.at(2).template value<QList<int>>().contains(availabilityRole);
    });
    QVERIFY(availabilityChanged);
}

void MediaRendererModelShould::give_the_name_and_playback_state_of_the_renderer()
{
    auto modelTester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};
    Q_EMIT mServiceProvider->serviceConnected(kitchenUsn);

    QCOMPARE(name(0), QStringLiteral("Kitchen"));
    QCOMPARE(mModel->data(mModel->index(0), playbackStateRole).toInt(), static_cast<int>(Renderer::State::NoMedia));

    mRendererFactory->renderer(QStringLiteral("Kitchen"))->setDeviceState(MediaDevice::State::Playing);

    QCOMPARE(mModel->data(mModel->index(0), playbackStateRole).toInt(), static_cast<int>(Renderer::State::Playing));
}

void MediaRendererModelShould::notify_about_a_changed_playback_state()
{
    auto modelTester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};
    Q_EMIT mServiceProvider->serviceConnected(kitchenUsn);
    Q_EMIT mServiceProvider->serviceConnected(bathroomUsn);
    auto dataChangedSpy = QSignalSpy{mModel.get(), &MediaRendererModel::dataChanged};

    mRendererFactory->renderer(QStringLiteral("Kitchen"))->setDeviceState(MediaDevice::State::PausedPlayback);

    QCOMPARE(dataChangedSpy.size(), 1);
    QCOMPARE(dataChangedSpy.at(0).at(0).value<QModelIndex>().row(), 1);
    QCOMPARE(dataChangedSpy.at(0).at(1).value<QModelIndex>().row(), 1);
    QCOMPARE(dataChangedSpy.at(0).at(2).value<QList<int>>(), QList<int>({playbackStateRole}));
    QCOMPARE(mModel->data(mModel->index(1), playbackStateRole).toInt(), static_cast<int>(Renderer::State::Paused));
}

void MediaRendererModelShould::give_the_manufacturer_model_and_address_of_the_renderer()
{
    auto modelTester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};
    Q_EMIT mServiceProvider->serviceConnected(kitchenUsn);
    Q_EMIT mServiceProvider->serviceConnected(bathroomUsn);

    QCOMPARE(name(1), QStringLiteral("Kitchen"));
    QCOMPARE(mModel->data(mModel->index(1), manufacturerRole).toString(), QStringLiteral("Denon"));
    QCOMPARE(mModel->data(mModel->index(1), modelNameRole).toString(), QStringLiteral("HEOS 1"));
    QCOMPARE(mModel->data(mModel->index(1), addressRole).toString(), QStringLiteral("192.168.1.42"));

    QCOMPARE(name(0), QStringLiteral("Bathroom"));
    QCOMPARE(mModel->data(mModel->index(0), manufacturerRole).toString(), QString{});
    QCOMPARE(mModel->data(mModel->index(0), modelNameRole).toString(), QString{});
    QCOMPARE(mModel->data(mModel->index(0), addressRole).toString(), QString{});
}

void MediaRendererModelShould::order_the_renderers_alphabetically_by_name()
{
    auto modelTester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};

    Q_EMIT mServiceProvider->serviceConnected(livingRoomUsn);
    Q_EMIT mServiceProvider->serviceConnected(kitchenUsn);
    Q_EMIT mServiceProvider->serviceConnected(bathroomUsn);

    auto const expNames =
        QStringList{QStringLiteral("Bathroom"), QStringLiteral("Kitchen"), QStringLiteral("living room")};
    QCOMPARE(rendererNames(), expNames);
}

void MediaRendererModelShould::keep_the_renderers_ordered_when_renderers_appear_and_disappear()
{
    auto modelTester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};
    Q_EMIT mServiceProvider->serviceConnected(livingRoomUsn);
    Q_EMIT mServiceProvider->serviceConnected(kitchenUsn);
    Q_EMIT mServiceProvider->serviceConnected(bathroomUsn);

    Q_EMIT mServiceProvider->serviceDisconnected(kitchenUsn);
    QCOMPARE(rendererNames(),
             (QStringList{QStringLiteral("Bathroom"), QStringLiteral("living room"), QStringLiteral("Kitchen")}));

    auto rowsInsertedSpy = QSignalSpy{mModel.get(), &MediaRendererModel::rowsInserted};
    Q_EMIT mServiceProvider->serviceConnected(kitchenUsn);
    QCOMPARE(rendererNames(),
             (QStringList{QStringLiteral("Bathroom"), QStringLiteral("Kitchen"), QStringLiteral("living room")}));
    QCOMPARE(rowsInsertedSpy.size(), 0);
}

void MediaRendererModelShould::have_no_active_renderer_at_start()
{
    Q_EMIT mServiceProvider->serviceConnected(kitchenUsn);

    QCOMPARE(mModel->activeRenderer(), nullptr);
    QCOMPARE(isActive(0), false);
}

void MediaRendererModelShould::activate_the_renderer_of_the_passed_index()
{
    Q_EMIT mServiceProvider->serviceConnected(kitchenUsn);
    auto activeRendererChangedSpy = QSignalSpy{mModel.get(), &MediaRendererModel::activeRendererChanged};
    auto dataChangedSpy = QSignalSpy{mModel.get(), &MediaRendererModel::dataChanged};

    mModel->activateRenderer(mModel->index(0));

    QCOMPARE_NE(mModel->property("activeRenderer").value<std::shared_ptr<Renderer>>(), nullptr);
    QCOMPARE(mModel->activeRenderer()->name(), QStringLiteral("Kitchen"));
    QCOMPARE(activeRendererChangedSpy.size(), 1);
    QCOMPARE(dataChangedSpy.size(), 1);
    QCOMPARE(dataChangedSpy.at(0).at(0).value<QModelIndex>().row(), 0);
    QCOMPARE(dataChangedSpy.at(0).at(1).value<QModelIndex>().row(), 0);
    QCOMPARE(dataChangedSpy.at(0).at(2).value<QList<int>>(), QList<int>({activeRole}));
    QCOMPARE(isActive(0), true);
}

void MediaRendererModelShould::switch_the_active_renderer_without_stopping_the_previous_one()
{
    Q_EMIT mServiceProvider->serviceConnected(kitchenUsn);
    Q_EMIT mServiceProvider->serviceConnected(bathroomUsn);
    auto* kitchen = mRendererFactory->renderer(QStringLiteral("Kitchen"));
    kitchen->setPauseEnabled(true);
    kitchen->setDeviceState(MediaDevice::State::Playing);
    mModel->activateRenderer(mModel->index(1));
    auto activeRendererChangedSpy = QSignalSpy{mModel.get(), &MediaRendererModel::activeRendererChanged};
    auto dataChangedSpy = QSignalSpy{mModel.get(), &MediaRendererModel::dataChanged};

    mModel->activateRenderer(mModel->index(0));

    QCOMPARE(mModel->activeRenderer()->name(), QStringLiteral("Bathroom"));
    QCOMPARE(activeRendererChangedSpy.size(), 1);
    QCOMPARE(isActive(0), true);
    QCOMPARE(isActive(1), false);
    auto changedRows = QList<int>{};
    for (auto const& args : std::as_const(dataChangedSpy)) {
        QCOMPARE(args.at(0).value<QModelIndex>().row(), args.at(1).value<QModelIndex>().row());
        QCOMPARE(args.at(2).value<QList<int>>(), QList<int>({activeRole}));
        changedRows.append(args.at(0).value<QModelIndex>().row());
    }
    std::sort(changedRows.begin(), changedRows.end());
    QCOMPARE(changedRows, (QList<int>{0, 1}));
    QCOMPARE(kitchen->isStopCalled(), false);
    QCOMPARE(kitchen->isPauseCalled(), false);
    QCOMPARE(mModel->data(mModel->index(1), playbackStateRole).toInt(), static_cast<int>(Renderer::State::Playing));
}

void MediaRendererModelShould::ignore_activating_the_active_renderer_again()
{
    Q_EMIT mServiceProvider->serviceConnected(kitchenUsn);
    mModel->activateRenderer(mModel->index(0));
    auto activeRendererChangedSpy = QSignalSpy{mModel.get(), &MediaRendererModel::activeRendererChanged};
    auto dataChangedSpy = QSignalSpy{mModel.get(), &MediaRendererModel::dataChanged};

    mModel->activateRenderer(mModel->index(0));

    QCOMPARE(activeRendererChangedSpy.size(), 0);
    QCOMPARE(dataChangedSpy.size(), 0);
    QCOMPARE(isActive(0), true);
}

void MediaRendererModelShould::ignore_activating_an_invalid_index()
{
    Q_EMIT mServiceProvider->serviceConnected(kitchenUsn);
    auto activeRendererChangedSpy = QSignalSpy{mModel.get(), &MediaRendererModel::activeRendererChanged};

    mModel->activateRenderer(QModelIndex{});
    mModel->activateRenderer(mModel->index(1));

    QCOMPARE(activeRendererChangedSpy.size(), 0);
    QCOMPARE(mModel->activeRenderer(), nullptr);
}

void MediaRendererModelShould::clear_the_active_renderer_when_it_disconnects()
{
    auto modelTester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};
    Q_EMIT mServiceProvider->serviceConnected(kitchenUsn);
    mModel->activateRenderer(mModel->index(0));
    auto activeRendererChangedSpy = QSignalSpy{mModel.get(), &MediaRendererModel::activeRendererChanged};

    Q_EMIT mServiceProvider->serviceDisconnected(kitchenUsn);

    QCOMPARE(activeRendererChangedSpy.size(), 1);
    QCOMPARE(mModel->activeRenderer(), nullptr);
    QCOMPARE(isActive(0), false);
}

void MediaRendererModelShould::clear_the_active_renderer_when_its_device_does_not_answer()
{
    auto modelTester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};
    Q_EMIT mServiceProvider->serviceConnected(kitchenUsn);
    mModel->activateRenderer(mModel->index(0));
    auto activeRendererChangedSpy = QSignalSpy{mModel.get(), &MediaRendererModel::activeRendererChanged};
    auto* device = mRendererFactory->renderer(QStringLiteral("Kitchen"));
    device->setVolumeEnabled(true);
    auto volumeCall = device->setVolumeCall();
    mModel->activeRenderer()->setVolume(25);

    volumeCall->setDeviceUnreachable();
    Q_EMIT volumeCall->finished();

    QTRY_VERIFY(mModel->activeRenderer() == nullptr);
    QCOMPARE(activeRendererChangedSpy.size(), 1);
    QCOMPARE(isActive(0), false);
}

void MediaRendererModelShould::keep_the_active_renderer_when_another_renderer_disconnects()
{
    auto modelTester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};
    Q_EMIT mServiceProvider->serviceConnected(kitchenUsn);
    Q_EMIT mServiceProvider->serviceConnected(bathroomUsn);
    mModel->activateRenderer(mModel->index(1));
    auto activeRendererChangedSpy = QSignalSpy{mModel.get(), &MediaRendererModel::activeRendererChanged};

    Q_EMIT mServiceProvider->serviceDisconnected(bathroomUsn);

    QCOMPARE(activeRendererChangedSpy.size(), 0);
    QCOMPARE(mModel->activeRenderer()->name(), QStringLiteral("Kitchen"));
    QCOMPARE(isActive(0), true);
}

void MediaRendererModelShould::be_scanning_after_start_until_the_discovery_is_finished()
{
    auto scanningChangedSpy = QSignalSpy{mModel.get(), &MediaRendererModel::scanningChanged};
    QCOMPARE(mModel->property("scanning").toBool(), true);

    Q_EMIT mServiceProvider->searchFinished();

    QCOMPARE(mModel->isScanning(), false);
    QCOMPARE(scanningChangedSpy.size(), 1);
}

void MediaRendererModelShould::start_a_new_discovery_on_rescan()
{
    Q_EMIT mServiceProvider->searchFinished();
    auto scanningChangedSpy = QSignalSpy{mModel.get(), &MediaRendererModel::scanningChanged};

    mModel->rescan();

    QCOMPARE(mServiceProvider->searchCount(), 2);
    QCOMPARE(mModel->isScanning(), true);
    QCOMPARE(scanningChangedSpy.size(), 1);

    Q_EMIT mServiceProvider->searchFinished();

    QCOMPARE(mModel->isScanning(), false);
    QCOMPARE(scanningChangedSpy.size(), 2);
}

void MediaRendererModelShould::ignore_a_rescan_while_scanning()
{
    auto scanningChangedSpy = QSignalSpy{mModel.get(), &MediaRendererModel::scanningChanged};

    mModel->rescan();

    QCOMPARE(mServiceProvider->searchCount(), 1);
    QCOMPARE(mModel->isScanning(), true);
    QCOMPARE(scanningChangedSpy.size(), 0);
}

void MediaRendererModelShould::give_the_availability_of_the_renderer()
{
    Q_EMIT mServiceProvider->serviceConnected(kitchenUsn);

    QCOMPARE(availability(0), Renderer::Availability::Online);
}

void MediaRendererModelShould::list_the_remembered_renderers_as_offline_at_start()
{
    mStore = std::make_shared<TestHelper::InMemoryRendererStore>(QList{rememberedKitchen()});
    restart();

    QCOMPARE(mModel->rowCount(), 1);
    QCOMPARE(availability(0), Renderer::Availability::Offline);
    QCOMPARE(name(0), QStringLiteral("Old Kitchen"));
    QCOMPARE(mModel->data(mModel->index(0), manufacturerRole).toString(), QStringLiteral("Denon"));
    QCOMPARE(mModel->data(mModel->index(0), modelNameRole).toString(), QStringLiteral("HEOS 1"));
    QCOMPARE(mModel->data(mModel->index(0), addressRole).toString(), QStringLiteral("192.168.1.10"));
    QCOMPARE(mModel->activeRenderer(), nullptr);
}

void MediaRendererModelShould::bring_a_remembered_renderer_online_with_refreshed_details_when_it_is_discovered_again()
{
    mStore = std::make_shared<TestHelper::InMemoryRendererStore>(QList{rememberedKitchen()});
    restart();
    auto modelTester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};
    auto rowsInsertedSpy = QSignalSpy{mModel.get(), &MediaRendererModel::rowsInserted};
    auto dataChangedSpy = QSignalSpy{mModel.get(), &MediaRendererModel::dataChanged};

    Q_EMIT mServiceProvider->serviceConnected(kitchenUsn);

    QCOMPARE(mModel->rowCount(), 1);
    QCOMPARE(rowsInsertedSpy.size(), 0);
    QCOMPARE_GE(dataChangedSpy.size(), 1);
    QCOMPARE(availability(0), Renderer::Availability::Online);
    QCOMPARE(name(0), QStringLiteral("Kitchen"));
    QCOMPARE(mModel->data(mModel->index(0), addressRole).toString(), QStringLiteral("192.168.1.42"));
    auto expRemembered = rememberedKitchen();
    expRemembered.name = QStringLiteral("Kitchen");
    expRemembered.address = QStringLiteral("192.168.1.42");
    QCOMPARE(mStore->load(), QList{expRemembered});
}

void MediaRendererModelShould::remember_the_discovered_renderers_across_a_restart()
{
    Q_EMIT mServiceProvider->serviceConnected(kitchenUsn);
    Q_EMIT mServiceProvider->serviceConnected(bathroomUsn);

    restart();

    QCOMPARE(rendererNames(), (QStringList{QStringLiteral("Bathroom"), QStringLiteral("Kitchen")}));
    QCOMPARE(availability(0), Renderer::Availability::Offline);
    QCOMPARE(availability(1), Renderer::Availability::Offline);
    QCOMPARE(mModel->data(mModel->index(1), manufacturerRole).toString(), QStringLiteral("Denon"));
    QCOMPARE(mModel->data(mModel->index(1), modelNameRole).toString(), QStringLiteral("HEOS 1"));
    QCOMPARE(mModel->data(mModel->index(1), addressRole).toString(), QStringLiteral("192.168.1.42"));

    Q_EMIT mServiceProvider->serviceConnected(kitchenUsn);

    QCOMPARE(rendererNames(), (QStringList{QStringLiteral("Kitchen"), QStringLiteral("Bathroom")}));
    QCOMPARE(availability(0), Renderer::Availability::Online);
}

void MediaRendererModelShould::keep_the_last_known_address_of_an_offline_renderer_across_a_restart()
{
    Q_EMIT mServiceProvider->serviceConnected(kitchenUsn);
    Q_EMIT mServiceProvider->serviceDisconnected(kitchenUsn);

    restart();

    QCOMPARE(mModel->data(mModel->index(0), addressRole).toString(), QStringLiteral("192.168.1.42"));
}

void MediaRendererModelShould::order_online_renderers_before_offline_ones_and_alphabetically_within_each()
{
    mStore = std::make_shared<TestHelper::InMemoryRendererStore>(QList{rememberedKitchen(), rememberedAttic()});
    restart();
    auto modelTester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};

    Q_EMIT mServiceProvider->serviceConnected(livingRoomUsn);
    Q_EMIT mServiceProvider->serviceConnected(bathroomUsn);

    auto const expNames = QStringList{QStringLiteral("Bathroom"),
                                      QStringLiteral("living room"),
                                      QStringLiteral("Attic"),
                                      QStringLiteral("Old Kitchen")};
    QCOMPARE(rendererNames(), expNames);
}

void MediaRendererModelShould::move_a_renderer_behind_the_online_ones_when_it_goes_offline()
{
    auto modelTester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};
    Q_EMIT mServiceProvider->serviceConnected(livingRoomUsn);
    Q_EMIT mServiceProvider->serviceConnected(kitchenUsn);
    Q_EMIT mServiceProvider->serviceConnected(bathroomUsn);
    auto rowsMovedSpy = QSignalSpy{mModel.get(), &MediaRendererModel::rowsMoved};

    Q_EMIT mServiceProvider->serviceDisconnected(bathroomUsn);

    QCOMPARE(rendererNames(),
             (QStringList{QStringLiteral("Kitchen"), QStringLiteral("living room"), QStringLiteral("Bathroom")}));
    QCOMPARE(rowsMovedSpy.size(), 1);
    QCOMPARE(availability(2), Renderer::Availability::Offline);
}

void MediaRendererModelShould::ignore_activating_an_offline_renderer()
{
    mStore = std::make_shared<TestHelper::InMemoryRendererStore>(QList{rememberedKitchen()});
    restart();
    auto activeRendererChangedSpy = QSignalSpy{mModel.get(), &MediaRendererModel::activeRendererChanged};

    mModel->activateRenderer(mModel->index(0));

    QCOMPARE(activeRendererChangedSpy.size(), 0);
    QCOMPARE(mModel->activeRenderer(), nullptr);
    QCOMPARE(isActive(0), false);
}

void MediaRendererModelShould::forget_an_offline_renderer()
{
    mStore = std::make_shared<TestHelper::InMemoryRendererStore>(QList{rememberedKitchen(), rememberedAttic()});
    restart();
    auto modelTester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};
    auto rowsRemovedSpy = QSignalSpy{mModel.get(), &MediaRendererModel::rowsRemoved};

    mModel->forgetRenderer(mModel->index(0));

    QCOMPARE(rowsRemovedSpy.size(), 1);
    QCOMPARE(rendererNames(), QStringList{QStringLiteral("Old Kitchen")});
    QCOMPARE(mStore->load(), QList{rememberedKitchen()});
    restart();
    QCOMPARE(rendererNames(), QStringList{QStringLiteral("Old Kitchen")});
}

void MediaRendererModelShould::ignore_forgetting_an_online_renderer()
{
    Q_EMIT mServiceProvider->serviceConnected(kitchenUsn);
    auto rowsRemovedSpy = QSignalSpy{mModel.get(), &MediaRendererModel::rowsRemoved};

    mModel->forgetRenderer(mModel->index(0));

    QCOMPARE(rowsRemovedSpy.size(), 0);
    QCOMPARE(rendererNames(), QStringList{QStringLiteral("Kitchen")});
    QCOMPARE(mStore->load().size(), 1);
}

void MediaRendererModelShould::ignore_forgetting_an_invalid_index()
{
    mStore = std::make_shared<TestHelper::InMemoryRendererStore>(QList{rememberedKitchen()});
    restart();

    mModel->forgetRenderer(QModelIndex{});
    mModel->forgetRenderer(mModel->index(5));

    QCOMPARE(mModel->rowCount(), 1);
    QCOMPARE(mStore->load().size(), 1);
}

void MediaRendererModelShould::remember_a_forgotten_renderer_again_when_it_is_discovered_again()
{
    mStore = std::make_shared<TestHelper::InMemoryRendererStore>(QList{rememberedKitchen()});
    restart();
    auto modelTester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};
    mModel->forgetRenderer(mModel->index(0));

    Q_EMIT mServiceProvider->serviceConnected(kitchenUsn);

    QCOMPARE(rendererNames(), QStringList{QStringLiteral("Kitchen")});
    QCOMPARE(availability(0), Renderer::Availability::Online);
    QCOMPARE(mStore->load().size(), 1);
    QCOMPARE(mStore->load().at(0).name, QStringLiteral("Kitchen"));
}

} // namespace Shell

QTEST_MAIN(Shell::MediaRendererModelShould)
