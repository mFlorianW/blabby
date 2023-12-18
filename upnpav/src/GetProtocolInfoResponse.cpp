// SPDX-FileCopyrightText: 2023 Florian Weßel <florianwessel@gmx.net>
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

bool operator==(const Protocol &lhs, const Protocol &rhs) noexcept
{
    // clang-format off
    return (lhs.network == rhs.network or (lhs.network == "*" or rhs.network == "*")) and
           (lhs.protocol == rhs.protocol or (lhs.protocol == "*" or rhs.protocol == "*")) and 
           (lhs.contentFormat == rhs.contentFormat or (lhs.contentFormat == "*" or rhs.contentFormat == "*")) and
           (lhs.additionalInfo == rhs.additionalInfo or (lhs.additionalInfo== "*" or rhs.additionalInfo == "*"));
    // clang-format on
}

bool operator!=(const Protocol &lhs, const Protocol &rhs) noexcept
{
    return !(lhs == rhs);
}

std::optional<QVector<Protocol>> GetProtocolInfoResponse::parseProtocolResponse(QString const &rawResult) noexcept
{
    auto result = QVector<Protocol>{};
    if (rawResult.isEmpty()) {
        return result;
    }

    const auto elements = rawResult.split(",");
    for (auto const &proto : std::as_const(elements)) {
        const auto protoElements = proto.split(":");
        if (protoElements.size() < 4) {
            return std::nullopt;
        } else {
            auto proto = Protocol{.protocol = protoElements[0],
                                  .network = protoElements[1],
                                  .contentFormat = protoElements[2],
                                  .additionalInfo = protoElements[3]};
            result.push_back(proto);
        }
    }

    return result;
}

} // namespace UPnPAV
