// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "Source.hpp"
#include <QStringList>
#include <QtGlobal>

namespace Multimedia
{

class NavigationStack final
{
public:
    NavigationStack(Source& source);

    ~NavigationStack();

    Q_DISABLE_COPY_MOVE(NavigationStack)

    QString const& path();

    void navigateForward();

    void navigateBack(qsizetype minimumItemCount);

private:
    Source& mSource;
    QString mPath;
    QStringList mPathStack;
    qsizetype mNavigationIndex{0};
    // The navigation index before navigating back or forward, restored when that navigation fails.
    qsizetype mPreviousNavigationIndex{0};
    bool mNavigationAction{false};
};

} // namespace Multimedia
