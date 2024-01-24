// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ResponseSerializer.hpp"
#include <QTcpSocket>

namespace Http
{

namespace
{
constexpr auto* respHeader = "HTTP/1.1 %1 %2\r\n"
                             "User-Agent: Blabby/0.2.0\r\n" // TODO: replace with the concret software version
                             "Connection: Close\r\n";
}

ResponseSerializer::ResponseSerializer(ServerResponse serverResponse)
    : QObject{}
    , mResponse{std::move(serverResponse)}
{
}

ResponseSerializer::~ResponseSerializer() = default;

void ResponseSerializer::serialize() noexcept
{
    auto response = QString{respHeader}
                        .arg(QString::number(static_cast<qint32>(mResponse.statusCode())),
                             QVariant::fromValue<Response::StatusCode>(mResponse.statusCode()).toString().toUpper())
                        .toUtf8();
    auto const& headers = mResponse.headers();
    auto const& body = mResponse.body();
    for (auto header = headers.cbegin(); header != headers.cend(); ++header) {
        response.append(header.key());
        response.append(": ");
        response.append(header.value());
        response.append("\r\n");
    }
    response.append("Content-Length: ");
    response.append(QString::number(body.size()).toUtf8());
    response.append("\r\n");
    response.append("\r\n");
    if (not body.isEmpty()) {
        response.append(body);
    }
    Q_EMIT responseSerialized(response);
}

} // namespace Http
