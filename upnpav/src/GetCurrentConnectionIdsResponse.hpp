// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "SCPDAction.hpp"
#include "ServiceControlPointDefinition.hpp"
#include "blabbyupnpav_export.h"
#include <QString>
#include <QVector>

namespace UPnPAV
{

/**
 * Converts the response of GetCurrentConnectionIds call into a list of quint32 values.
 */
class BLABBYUPNPAV_EXPORT GetCurrentConnectionIdsResponse final
{
public:
    GetCurrentConnectionIdsResponse(QString xmlResponse, ServiceControlPointDefinition scpd, SCPDAction action);

    QVector<quint32> currentConnectionIds() const noexcept;

private:
    QVector<quint32> mConnectionIds;
};
} // namespace UPnPAV
