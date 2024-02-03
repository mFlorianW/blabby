// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "EventPropertyReader.hpp"
#include "LoggingCategories.hpp"
#include <QXmlStreamReader>

namespace UPnPAV
{

EventPropertyReader::EventPropertyReader(QString responseBody) noexcept
    : mResponseBody{std::move(responseBody)}
{
}

bool EventPropertyReader::read()
{
    auto xmlReader = QXmlStreamReader{mResponseBody};
    while (not xmlReader.atEnd() && not xmlReader.hasError()) {
        auto const token = xmlReader.readNext();
        if (token == QXmlStreamReader::StartElement && xmlReader.name() == QStringLiteral("property")) {
            static_cast<void>(xmlReader.readNextStartElement());
            mProperties.insert(xmlReader.name().toString(), xmlReader.readElementText());
        }
    }
    if (xmlReader.hasError()) {
        qCCritical(upnpavEvent) << "Failed to read NOTIFY response. Error:" << xmlReader.errorString();
        return false;
    }
    return true;
}

std::optional<QString> EventPropertyReader::property(QString const& propertyName)
{
    if (mProperties.contains(propertyName)) {
        return mProperties.value(propertyName);
    }
    return std::nullopt;
}

} // namespace UPnPAV
