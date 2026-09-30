// SPDX-FileCopyrightText: 2024, 2026 All contributors
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
            auto volumeVariables = Variables{};
            auto muteVariables = Variables{};
            while (not xmlReader.atEnd() && not xmlReader.hasError() &&
                   not(xmlReader.name() == QStringLiteral("/InstanceID"))) {
                static_cast<void>(xmlReader.readNextStartElement());
                if (not xmlReader.isStartElement()) {
                    continue;
                }
                auto const attributes = xmlReader.attributes();
                if (xmlReader.name() == QStringLiteral("Volume")) {
                    volumeVariables.insert(attributes.value(QStringLiteral("channel")).toString(),
                                           attributes.value("val").toString());
                } else if (xmlReader.name() == QStringLiteral("Mute")) {
                    muteVariables.insert(attributes.value(QStringLiteral("channel")).toString(),
                                         attributes.value("val").toString());
                } else {
                    variables.insert(xmlReader.name().toString(), attributes.value("val").toString());
                }
            }
            mInstanceVariables.insert(instanceId, variables);
            if (not volumeVariables.isEmpty()) {
                mInstanceVolumeVariables.insert(instanceId, volumeVariables);
            }
            if (not muteVariables.isEmpty()) {
                mInstanceMuteVariables.insert(instanceId, muteVariables);
            }
        }
    }
    if (xmlReader.hasError()) {
        qCCritical(upnpavEvent) << "Failed to read LastChange request" << xmlReader.errorString();
    }
    return true;
}

InstanceVariables const& LastChangeReader::instanceVariables() const noexcept
{
    return mInstanceVariables;
}

InstanceVariables const& LastChangeReader::instanceVolumeVariables() const noexcept
{
    return mInstanceVolumeVariables;
}

InstanceVariables const& LastChangeReader::instanceMuteVariables() const noexcept
{
    return mInstanceMuteVariables;
}

} // namespace UPnPAV
