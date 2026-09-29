// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "Source.hpp"
#include <QHash>

namespace Multimedia::TestHelper
{

class TestSource : public Multimedia::Source
{
    Q_OBJECT
public:
    TestSource(QString name, QString iconUrl);
    ~TestSource() override;
    Q_DISABLE_COPY_MOVE(TestSource)

    void navigateTo(QString const& path) noexcept override;

    QString const& lastNavigatedPath() const noexcept;

    /**
     * Gives how often a navigation was requested.
     */
    qsizetype navigationCount() const noexcept;

    /**
     * When true, a navigation only finishes on @ref finishPendingNavigation(), like a Source waiting for a server.
     */
    void setHoldNavigations(bool hold) noexcept;

    /**
     * Finishes the navigation held back by @ref setHoldNavigations(bool).
     */
    void finishPendingNavigation() noexcept;

    /**
     * Fails the navigation held back by @ref setHoldNavigations(bool), the Items stay unchanged.
     */
    void failPendingNavigation() noexcept;

private:
    void finishNavigation(QString const& path) noexcept;

    QHash<QString, Items> mItems;
    QString mLastNavigationPath;
    qsizetype mNavigationCount{0};
    bool mHoldNavigations{false};
    QString mPendingPath;
};

} // namespace Multimedia::TestHelper
