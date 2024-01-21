// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ResponseSerializer.hpp"
#include <QTcpSocket>

namespace Http
{

namespace
{
constexpr auto* responseTemplate = "HTTP/1.1 %1 %2\r\n"
                                   "User-Agent: Blabby/0.2.0\r\n" // TODO: replace with the concret software version
                                   "Connection: Close\r\n"
                                   "\r\n";
}

ResponseSerializer::ResponseSerializer(ServerResponse serverResponse)
    : QObject{}
    , mResponse{std::move(serverResponse)}
{
}

ResponseSerializer::~ResponseSerializer() = default;

void ResponseSerializer::serialize() noexcept
{
    auto response = QString{responseTemplate}
                        .arg(QString::number(static_cast<qint32>(mResponse.statusCode())),
                             QVariant::fromValue<Response::StatusCode>(mResponse.statusCode()).toString().toUpper())
                        .toUtf8();
    Q_EMIT responseSerialized(response);
}

} // namespace Http
