// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "LastChangeReader.hpp"
#include "private/LoggingCategories.hpp"
#include <QXmlStreamReader>

namespace UPnPAV
{

LastChangeReader::LastChangeReader(QString lastChange) noexcept
    : mLastChange{std::move(lastChange)}
{
}

bool LastChangeReader::read() noexcept
{
    auto xmlReader = QXmlStreamReader{mLastChange};
    while (not xmlReader.atEnd() && not xmlReader.hasError()) {
        auto const token = xmlReader.readNext();
        if (token == QXmlStreamReader::StartElement && xmlReader.name() == QStringLiteral("InstanceID")) {
            auto const instanceId = xmlReader.attributes().value("val").toString();
            auto variables = Variables{};
            while (not xmlReader.atEnd() && not xmlReader.hasError() &&
                   not(xmlReader.name() == QStringLiteral("/InstanceID"))) {
                static_cast<void>(xmlReader.readNextStartElement());
                if (xmlReader.isStartElement()) {
                    variables.insert(xmlReader.name().toString(), xmlReader.attributes().value("val").toString());
                }
            }
            mInstanceVariables.insert(instanceId, variables);
        }
    }
    if (xmlReader.hasError()) {
        qCCritical(upnpavEvent) << "Failed to read LastChange request" << xmlReader.errorString();
    }
    return true;
}

InstanceVariables const& LastChangeReader::instanceVariables() noexcept
{
    return mInstanceVariables;
}

} // namespace UPnPAV
