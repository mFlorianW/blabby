// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ClientConnection.hpp"

namespace Http
{

ClientConnection::ClientConnection(QTcpSocket* connection)
    : QObject{}
    , mSocket{connection}
{
}

ClientConnection::~ClientConnection()
{
    if (mReadThread.isRunning()) {
        stopReadThread();
    }
};

void ClientConnection::readRequest() noexcept
{
    mReader = std::make_unique<RequestReader>(mSocket);
    mReader->moveToThread(&mReadThread);
    connect(mReader.get(), &RequestReader::requestRead, this, [this] {
        stopReadThread();
        mRequest = mReader->serverRequest();
        mReader = nullptr;
        Q_EMIT requestReceived(mRequest);
    });
    connect(&mReadThread, &QThread::started, mReader.get(), &RequestReader::readRequest);
    mReadThread.start();
}

void ClientConnection::stopReadThread()
{
    mReadThread.quit();
    mReadThread.wait();
}

} // namespace Http
