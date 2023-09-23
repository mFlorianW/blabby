// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include <QString>
#include <QVector>

namespace UPnPAV
{

/**
 * Converts the response of GetCurrentConnectionIds call into a list of quint32 values.
 */
class CurrentConnectionIdsResult
{
public:
    CurrentConnectionIdsResult(const QString &xmlResponse);

    QVector<quint32> getCurrentConnectionIds() const noexcept;

private:
    QVector<quint32> mConnectionIds;
};
} // namespace UPnPAV
