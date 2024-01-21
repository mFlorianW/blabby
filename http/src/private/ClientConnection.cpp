// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ClientConnection.hpp"
#include "private/LoggingCategories.hpp"

namespace Http
{

ClientConnection::ClientConnection(qintptr socketDesc)
    : QObject{}
{
    mSocket.setSocketDescriptor(socketDesc);
    connect(&mSocket, &QTcpSocket::readyRead, this, &ClientConnection::readRequest);
    connect(&mSocket, &QTcpSocket::bytesWritten, this, [this](auto bytes) {
        if (mResponseSize == bytes) {
            mResponseSize = qsizetype{0};
            Q_EMIT responseSent(this);
        }
    });
    qCDebug(httpServer) << "New client connection from:" << mSocket.peerAddress().toString() << mSocket.peerPort();
}

ClientConnection::~ClientConnection()
{
    if (mReadThread.isRunning()) {
        stopThread(mReadThread);
    }

    if (mWriteThread.isRunning()) {
        stopThread(mWriteThread);
    }

    if (mSocket.state() != QTcpSocket::ClosingState) {
        mSocket.close();
    }
};

void ClientConnection::close()
{
    mSocket.close();
}

void ClientConnection::readRequest() noexcept
{
    if (mSocket.bytesAvailable() == 0) {
        return;
    }
    auto request = mSocket.readAll();
    mReader = std::make_unique<RequestDeserializer>(request);
    mReader->moveToThread(&mReadThread);
    connect(mReader.get(), &RequestDeserializer::requestRead, this, [this] {
        stopThread(mReadThread);
        mRequest = mReader->serverRequest();
        mReader = nullptr;
        Q_EMIT requestReceived(mRequest, this);
    });
    connect(&mReadThread, &QThread::started, mReader.get(), &RequestDeserializer::readRequest);
    mReadThread.start();
    qCDebug(httpServer) << "Read request for connection:" << mSocket.peerAddress().toString() << mSocket.peerPort();
}

void ClientConnection::sendResponse(ServerResponse const& serverResponse) noexcept
{
    mWriter = std::make_unique<ResponseSerializer>(serverResponse);
    mWriter->moveToThread(&mWriteThread);
    connect(mWriter.get(), &ResponseSerializer::responseSerialized, this, [this](auto const& response) {
        stopThread(mWriteThread);
        mResponseSize = response.size();
        mSocket.write(response);
    });
    connect(&mWriteThread, &QThread::started, mWriter.get(), &ResponseSerializer::serialize);
    mWriteThread.start();
}

void ClientConnection::stopThread(QThread& thread)
{
    thread.quit();
    thread.wait();
}

} // namespace Http
