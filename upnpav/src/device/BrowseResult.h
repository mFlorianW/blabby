// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once
#include "MediaServerObject.h"
#include "SCPDAction.h"
#include "ServiceControlPointDefinition.h"
#include "blabbyupnpav_export.h"
#include <QVector>

class QXmlStreamReader;

namespace UPnPAV
{

class BLABBYUPNPAV_EXPORT BrowseResult
{
public:
    BrowseResult(QString xmlResponse, ServiceControlPointDefinition scpd, SCPDAction action);

    quint32 totalMatches() const noexcept;

    quint32 numberReturned() const noexcept;

    quint32 updateId() const noexcept;

    const QVector<MediaServerObject> &objects() const noexcept;

private:
    void readDidlDescription(const QString &didlDescription);
    MediaServerObject readDidlObjectDescription(QXmlStreamReader &streamReader);

private:
    quint32 mNumberReturned{0};
    quint32 mTotalMatches{0};
    quint32 mUpdateId{0};

    QVector<MediaServerObject> m_objects;
};

} // namespace UPnPAV
