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
} // namespace

MediaRendererModelShould::~MediaRendererModelShould() = default;

void MediaRendererModelShould::init()
{
    auto sProvider = std::make_unique<ServiceProviderDouble>();
    sProvider->addDeviceDescription(kitchenUsn, validRendererDeviceDescription(QStringLiteral("Kitchen")));
    sProvider->addDeviceDescription(bathroomUsn, validRendererDeviceDescription(QStringLiteral("Bathroom")));
    sProvider->addDeviceDescription(livingRoomUsn, validRendererDeviceDescription(QStringLiteral("living room")));
    mServiceProvider = sProvider.get();
    auto rendererFactory = std::make_unique<MediaRendererDoubleFactory>();
    mRendererFactory = rendererFactory.get();
    auto rProvider = std::make_unique<RendererProvider>(std::move(sProvider), std::move(rendererFactory));
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

void MediaRendererModelShould::give_correct_display_roles_for_the_ui()
{
    auto const expRoles = QHash<int, QByteArray>{
        std::make_pair(nameRole, QByteArray{"name"}),
        std::make_pair(playbackStateRole, QByteArray{"playbackState"}),
        std::make_pair(activeRole, QByteArray{"active"}),
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

void MediaRendererModelShould::decrease_the_rowCount_on_disconnected_mediarenderer()
{
    auto modelTester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};

    Q_EMIT mServiceProvider->serviceConnected(kitchenUsn);
    auto rowCount = mModel->rowCount();
    QCOMPARE(rowCount, 1);

    Q_EMIT mServiceProvider->serviceDisconnected(kitchenUsn);
    rowCount = mModel->rowCount();
    QCOMPARE(rowCount, 0);
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
    QCOMPARE(rendererNames(), (QStringList{QStringLiteral("Bathroom"), QStringLiteral("living room")}));

    auto rowsInsertedSpy = QSignalSpy{mModel.get(), &MediaRendererModel::rowsInserted};
    Q_EMIT mServiceProvider->serviceConnected(kitchenUsn);
    QCOMPARE(rendererNames(),
             (QStringList{QStringLiteral("Bathroom"), QStringLiteral("Kitchen"), QStringLiteral("living room")}));
    QCOMPARE(rowsInsertedSpy.size(), 1);
    QCOMPARE(rowsInsertedSpy.at(0).at(1).toInt(), 1);
    QCOMPARE(rowsInsertedSpy.at(0).at(2).toInt(), 1);
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

} // namespace Shell

QTEST_MAIN(Shell::MediaRendererModelShould)
