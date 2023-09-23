// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "CurrentConnectionIdsResult.h"
#include <QDebug>
#include <QXmlStreamReader>

namespace UPnPAV
{

CurrentConnectionIdsResult::CurrentConnectionIdsResult(const QString &xmlResponse)
{
    auto responseReader = QXmlStreamReader{xmlResponse};
    while (responseReader.readNext() && !responseReader.atEnd() && !responseReader.hasError())
    {
        if (responseReader.isStartElement() && responseReader.name() == QStringLiteral("CurrentConnectionIDs"))
        {
            const auto idList = responseReader.readElementText().split(",");
            for (auto const &rawId : std::as_const(idList))
            {
                bool ok = false;
                const auto id = rawId.toInt(&ok, 10);
                if (ok)
                {
                    mConnectionIds.push_back(id);
                }
                else
                {
                    qCritical() << "Failed to convert connection id " << rawId << rawId;
                }
            }
        }
    }

    if (responseReader.hasError())
    {
        qCritical() << "Failed GetCurrentConnectionIds response";
        qCritical() << xmlResponse;
        qCritical() << "XML Error:" << responseReader.errorString();
    }
}

QVector<quint32> CurrentConnectionIdsResult::getCurrentConnectionIds() const noexcept
{
    return mConnectionIds;
}

} // namespace UPnPAV
