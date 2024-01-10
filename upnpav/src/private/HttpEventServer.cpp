// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "HttpEventServer.hpp"
#include <QUrl>

namespace UPnPAV
{
namespace
{
constexpr auto serverIp = "BLABBY_EVENT_SERVER_IP";
constexpr auto serverPort = "BLABBY_EVENT_SERVER_PORT";
} // namespace

HttpEventServer::HttpEventServer() = default;
HttpEventServer::~HttpEventServer() = default;

QUrl HttpEventServer::callbackHost() noexcept
{
    return QString{"http://%1:%2"}.arg(mServerAddress.toString(), QString::number(mServerPort));
}

HttpEventServer& HttpEventServer::instance()
{
    static bool initialized = false;
    static HttpEventServer server;
    if (not initialized) {
        if (qEnvironmentVariableIsSet(serverIp)) {
            auto address = QHostAddress{qEnvironmentVariable(serverIp)};
            if (address.isNull()) {
                qFatal() << "Server IP evironment" << serverIp
                         << "has incorrect value:" << qEnvironmentVariable(serverIp);
            }
            server.mServerAddress = address;
        }

        if (qEnvironmentVariableIsSet(serverPort)) {
            bool ok = false;
            auto const port = static_cast<quint16>(qEnvironmentVariable(serverPort).toUInt(&ok));
            if (not ok) {
                qFatal() << "Server Port evironment variable" << serverPort
                         << "has incorrect value:" << qEnvironmentVariable(serverPort);
            }
            server.mServerPort = port;
        }
    }

    return server;
}

} // namespace UPnPAV
