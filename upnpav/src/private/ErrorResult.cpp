// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ErrorResult.hpp"

#include <QDebug>
#include <QXmlStreamReader>

namespace UPnPAV
{

ErrorResult::ErrorResult(const QString &rawMessage)
{
    if (rawMessage.isEmpty()) {
        return;
    }

    QXmlStreamReader errorReader{rawMessage};

    while (errorReader.readNext() && !errorReader.hasError() && !errorReader.atEnd()) {
        if (errorReader.isStartElement() && errorReader.name() == QStringLiteral("errorCode")) {
            bool ok = false;
            m_errorCode = errorReader.readElementText().toInt(&ok);

            if (!ok) {
                m_errorCode = 0;
            }
        }

        if (errorReader.isStartElement() && errorReader.name() == QStringLiteral("errorDescription")) {
            m_errorDescription = errorReader.readElementText();
        }
    }

    if (errorReader.hasError()) {
        qInfo() << "Failed to parse error XML result:" << errorReader.errorString();
    }
}

qint32 ErrorResult::errorCode() const noexcept
{
    return m_errorCode;
}

QString ErrorResult::errorDescription() const noexcept
{
    return m_errorDescription;
}

} // namespace UPnPAV
