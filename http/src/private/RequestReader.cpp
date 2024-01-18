// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "RequestReader.hpp"
#include <QMutexLocker>
#include <qthread.h>

namespace Http
{

RequestReader::RequestReader(QTcpSocket* socket)
    : QObject{}
    , mSocket{socket}
{
}

RequestReader::~RequestReader() = default;

ServerRequest RequestReader::serverRequest() const noexcept
{
    auto locker = QMutexLocker{&mMutex};
    return mServerRequest;
}

void RequestReader::readRequest()
{
    Q_EMIT requestRead();
}

} // namespace Http
