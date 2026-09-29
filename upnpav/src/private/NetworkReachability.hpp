// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <QNetworkReply>

namespace UPnPAV
{

/**
 * Tells if a request failed because the device didn't answer, e.g. because it left the network.
 * A device that answers with an error response is reachable. Failures on our side, like a cancelled request or a
 * local network without connection, don't tell anything about the device.
 * @param error The error of the finished request.
 * @return True the device is unreachable, otherwise false.
 */
inline bool isDeviceUnreachable(QNetworkReply::NetworkError error) noexcept
{
    switch (error) {
    case QNetworkReply::ConnectionRefusedError:
    case QNetworkReply::RemoteHostClosedError:
    case QNetworkReply::HostNotFoundError:
    case QNetworkReply::TimeoutError:
    case QNetworkReply::UnknownNetworkError:
        return true;
    default:
        return false;
    }
}

} // namespace UPnPAV
