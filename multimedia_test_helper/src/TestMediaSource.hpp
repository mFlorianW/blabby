// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include "MediaSource.hpp"
#include <QHash>

namespace Multimedia::TestHelper
{

class TestMediaSource : public Multimedia::MediaSource
{
    Q_OBJECT
public:
    TestMediaSource(QString name, QString iconUrl);
    ~TestMediaSource() override;
    Q_DISABLE_COPY_MOVE(TestMediaSource)

    void navigateTo(QString const &path) noexcept override;

    QString const &lastNavigatedPath() const noexcept;

private:
    QHash<QString, MediaItems> mItems;
    QString mLastNavigationPath;
};

} // namespace Multimedia::TestHelper
