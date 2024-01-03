// SPDX-FileCopyrightText: 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "GetProtocolInfoResponse.hpp"
#include "private/LoggingCategories.hpp"
#include "private/ResponseReader.hpp"
#include <QDebug>
#include <QXmlStreamReader>
#include <optional>

namespace UPnPAV
{

GetProtocolInfoResponse::GetProtocolInfoResponse(const QString &xmlResponse)
{
    auto responseReader = QXmlStreamReader{xmlResponse};
    while (responseReader.readNext() && !responseReader.atEnd() && !responseReader.hasError()) {
        if (responseReader.isStartElement() && responseReader.name() == QStringLiteral("Source")) {
            auto const result = parseProtocolResponse(responseReader.readElementText());
            if (result.has_value()) {
                mSourceProtocols = result.value();
            } else {
                qCritical() << "Can't read source protocols. Ignore then.";
            }
        } else if (responseReader.isStartElement() && responseReader.name() == QStringLiteral("Sink")) {
            auto const result = parseProtocolResponse(responseReader.readElementText());
            if (result.has_value()) {
                mSinkProtocols = result.value();
            } else {
                qCritical() << "Can't read source protocols. Ignore then.";
            }
        }
    }

    if (responseReader.hasError()) {
        qCritical() << "Failed GetProtocolInfo response";
        qCritical() << xmlResponse;
        qCritical() << "XML Error:" << responseReader.errorString();
    }
}
GetProtocolInfoResponse::GetProtocolInfoResponse(QString const &xmlResponse,
                                                 ServiceControlPointDefinition scpd,
                                                 SCPDAction action)
{
    auto reader = ResponseReader{std::move(xmlResponse), std::move(scpd), std::move(action)};
    QObject::connect(
        &reader,
        &ResponseReader::stringValueRead,
        &reader,
        [this](QString const &elementName, QString &value, ResponseReader::ElementReadResult result) {
            if (elementName == QStringLiteral("Source") and result == ResponseReader::ElementReadResult::Ok) {
                auto const result = parseProtocolResponse(value);
                if (result.has_value()) {
                    mSourceProtocols = result.value();
                }
            } else if (elementName == QStringLiteral("Sink") and result == ResponseReader::ElementReadResult::Ok) {
                auto const result = parseProtocolResponse(value);
                if (result.has_value()) {
                    mSinkProtocols = result.value();
                }
            }
        });

    const auto result = reader.read();
    if (result != ResponseReader::ReadResult::Ok) {
        qCCritical(upnpavDevice) << "Failed to read GetProtocolInfo response. Response was:" << reader.response();
        return;
    }
}

const QVector<Protocol> &GetProtocolInfoResponse::sourceProtocols() const noexcept
{
    return mSourceProtocols;
}
const QVector<Protocol> &GetProtocolInfoResponse::sinkProtocols() const noexcept
{
    return mSinkProtocols;
}

std::optional<QVector<Protocol>> GetProtocolInfoResponse::parseProtocolResponse(QString const &rawResult) noexcept
{
    auto result = QVector<Protocol>{};
    if (rawResult.isEmpty()) {
        return result;
    }

    const auto elements = rawResult.split(",");
    for (auto const &rawProto : std::as_const(elements)) {
        const auto proto = Protocol::create(rawProto);
        if (proto.has_value()) {
            result.push_back(proto.value());
        }
    }
    return result;
}

} // namespace UPnPAV
