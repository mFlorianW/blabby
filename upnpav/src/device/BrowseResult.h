// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef BROWSERESULT_H
#define BROWSERESULT_H

#include "MediaServerObject.h"
#include "blabbyupnpav_export.h"

#include <QVector>

class QXmlStreamReader;

namespace UPnPAV
{

class BLABBYUPNPAV_EXPORT BrowseResult
{
public:
    BrowseResult(const QString xmlResponse);

    quint32 totalMatches() const noexcept;

    quint32 numberReturned() const noexcept;

    quint32 updateId() const noexcept;

    const QVector<MediaServerObject> &objects() const noexcept;

private:
    void readDidlDescription(const QString &didlDescription);
    MediaServerObject readDidlObjectDescription(QXmlStreamReader &streamReader);

private:
    quint32 m_numberReturned{0};
    quint32 m_totalMatches{0};
    quint32 m_updateId{0};

    QVector<MediaServerObject> m_objects;
};

} // namespace UPnPAV

#endif // BROWSERESULT_H
