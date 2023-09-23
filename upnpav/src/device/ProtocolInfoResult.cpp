// SPDX-FileCopyrightText: 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ProtocolInfoResult.h"
#include <QDebug>
#include <QXmlStreamReader>
#include <optional>

namespace UPnPAV
{

ProtocolInfoResult::ProtocolInfoResult(const QString &xmlResponse)
{
    auto responseReader = QXmlStreamReader{xmlResponse};
    while (responseReader.readNext() && !responseReader.atEnd() && !responseReader.hasError())
    {
        if (responseReader.isStartElement() && responseReader.name() == QStringLiteral("Source"))
        {
            auto const result = parseProtocolResponse(responseReader.readElementText());
            if (result.has_value())
            {
                mSourceProtocols = result.value();
            }
            else
            {
                qCritical() << "Can't read source protocols. Ignore then.";
            }
        }
        else if (responseReader.isStartElement() && responseReader.name() == QStringLiteral("Sink"))
        {
            auto const result = parseProtocolResponse(responseReader.readElementText());
            if (result.has_value())
            {
                mSinkProtocols = result.value();
            }
            else
            {
                qCritical() << "Can't read source protocols. Ignore then.";
            }
        }
    }

    if (responseReader.hasError())
    {
        qCritical() << "Failed GetProtocolInfo response";
        qCritical() << xmlResponse;
        qCritical() << "XML Error:" << responseReader.errorString();
    }
}

const QVector<Protocol> &ProtocolInfoResult::getSourceProtocols() const noexcept
{
    return mSourceProtocols;
}
const QVector<Protocol> &ProtocolInfoResult::getSinkProtocols() const noexcept
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

std::optional<QVector<Protocol>> ProtocolInfoResult::parseProtocolResponse(QString const &rawResult) noexcept
{
    auto result = QVector<Protocol>{};
    if (rawResult.isEmpty())
    {
        return result;
    }

    const auto elements = rawResult.split(",");
    for (auto const &proto : std::as_const(elements))
    {
        const auto protoElements = proto.split(":");
        if (protoElements.size() < 4)
        {
            return std::nullopt;
        }
        else
        {
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
