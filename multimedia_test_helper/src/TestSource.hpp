// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
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

    void navigateTo(QString const &path) noexcept override;

    QString const &lastNavigatedPath() const noexcept;

private:
    QHash<QString, Items> mItems;
    QString mLastNavigationPath;
};

} // namespace Multimedia::TestHelper
