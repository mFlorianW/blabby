// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
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

MediaRendererModelShould::~MediaRendererModelShould() = default;

void MediaRendererModelShould::init()
{
    auto sProvider = std::make_unique<UPnPAV::Doubles::ServiceProviderDouble>();
    sProvider->addDeviceDescription(QStringLiteral("RendererConnected"), UPnPAV::validRendererDeviceDescription());
    mServiceProvider = sProvider.get();
    auto rProvider = std::make_unique<RendererProvider>(std::move(sProvider));
    mModel = std::make_unique<MediaRendererModel>(std::move(rProvider));
}

void MediaRendererModelShould::give_correct_display_roles_for_the_ui()
{
    auto const expRoles = QHash<int, QByteArray>{
        std::make_pair(static_cast<int>(MediaRendererModel::DisplayRole::MediaRendererTitle),
                       QByteArray{"mediaRendererTitle"}),
        std::make_pair(static_cast<int>(MediaRendererModel::DisplayRole::MediaRendererIconUrl),
                       QByteArray{"mediaRendererIconUrl"}),
        std::make_pair(static_cast<int>(MediaRendererModel::DisplayRole::MediaRendererActive),
                       QByteArray{"mediaRendererActive"}),
    };

    const auto roleNames = mModel->roleNames();

    QCOMPARE(roleNames, expRoles);
}

void MediaRendererModelShould::start_a_mediarenderer_discover_on_init()
{
    QCOMPARE(mServiceProvider->isSearchTriggered(), true);
}

void MediaRendererModelShould::increase_the_rowCount_on_new_connected_mediarenderer()
{
    auto modelTester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};

    Q_EMIT mServiceProvider->serviceConnected("RendererConnected");
    const auto rowCount = mModel->rowCount();

    QCOMPARE(rowCount, 1);
}

void MediaRendererModelShould::decrease_the_rowCount_on_disconnected_mediarenderer()
{
    auto modelTester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};

    Q_EMIT mServiceProvider->serviceConnected("RendererConnected");
    auto rowCount = mModel->rowCount();
    QCOMPARE(rowCount, 1);

    Q_EMIT mServiceProvider->serviceDisconnected("RendererConnected");
    rowCount = mModel->rowCount();
    QCOMPARE(rowCount, 0);
}

void MediaRendererModelShould::give_correct_data_for_the_ui()
{
    auto modelTester = QAbstractItemModelTester{mModel.get(), QAbstractItemModelTester::FailureReportingMode::QtTest};
    Q_EMIT mServiceProvider->serviceConnected("RendererConnected");

    const auto rendererName =
        mModel->data(mModel->index(0), static_cast<int>(MediaRendererModel::DisplayRole::MediaRendererTitle))
            .toString();
    const auto rendererIconUrl =
        mModel->data(mModel->index(0), static_cast<int>(MediaRendererModel::DisplayRole::MediaRendererIconUrl))
            .toString();

    QCOMPARE(rendererName, QStringLiteral("MediaRenderer"));
    QCOMPARE(rendererIconUrl, QStringLiteral("http://localhost:8200/icons/sm.png"));
}

void MediaRendererModelShould::set_active_renderer_property_correctly()
{
    auto activeRendererChangedSpy = QSignalSpy{mModel.get(), &MediaRendererModel::activeRendererChanged};
    auto dataChangedSpy = QSignalSpy{mModel.get(), &MediaRendererModel::dataChanged};
    Q_EMIT mServiceProvider->serviceConnected("RendererConnected");

    mModel->activateRenderer(mModel->index(0));

    QCOMPARE_NE(mModel->property("activeRenderer").value<std::shared_ptr<Renderer>>(), nullptr);
    QCOMPARE(activeRendererChangedSpy.size(), 1);
    QCOMPARE(dataChangedSpy.size(), 1);
    QCOMPARE(dataChangedSpy.at(0).size(), 3);
    QCOMPARE(dataChangedSpy.at(0).at(0).value<QModelIndex>().row(), 0);
    QCOMPARE(dataChangedSpy.at(0).at(1).value<QModelIndex>().row(), 0);
    QCOMPARE(static_cast<MediaRendererModel::DisplayRole>(dataChangedSpy.at(0).at(2).value<QList<int>>().at(0)),
             MediaRendererModel::DisplayRole::MediaRendererActive);
    auto const activeState =
        mModel->data(mModel->index(0), static_cast<int>(MediaRendererModel::DisplayRole::MediaRendererActive)).toBool();
    QCOMPARE(activeState, true);
}

} // namespace Shell

QTEST_MAIN(Shell::MediaRendererModelShould)
