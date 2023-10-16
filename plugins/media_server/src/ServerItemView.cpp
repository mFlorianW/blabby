// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later
#include "ServerItemView.h"
#include "BrowseResponse.h"
#include "MediaServer.h"
#include "PendingSoapCall.h"
#include "ServerItemModel.h"

namespace MediaServer::Plugin
{

ServerItemView::ServerItemView(ServerItemModel *model)
    : QObject()
    , mServerItemModel{model}
{
}

ServerItemView::~ServerItemView() = default;

UPnPAV::MediaServer *ServerItemView::mediaServer() const noexcept
{
    return mMediaServer;
}

void ServerItemView::setMediaServer(UPnPAV::MediaServer *mediaServer) noexcept
{
    if (mediaServer == nullptr)
    {
        return;
    }

    mMediaServer = mediaServer;
    requestStorageFolder("0");
    Q_EMIT mediaServerChanged();
}

ServerItemModel *ServerItemView::serverItemModel() const noexcept
{
    return mServerItemModel;
}

void MediaServer::Plugin::ServerItemView::requestFolder(const QString &id)
{
    mPendingSoapCall = mMediaServer->browse(id, UPnPAV::MediaServer::DirectChildren, "", "");
    connect(mPendingSoapCall.get(), &UPnPAV::PendingSoapCall::finished, this, &ServerItemView::onBrowsRequestFinished);
}

void ServerItemView::requestStorageFolder(const QString &id)
{
    if (mMediaServer == nullptr)
    {
        // TODO: add qcwarning
        return;
    }

    if (!mActiveId.isEmpty())
    {
        mIdStack.push(mActiveId);
    }

    mActiveId = id;
    requestFolder(mActiveId);
}

bool ServerItemView::goPreviousFolder()
{
    if (mIdStack.isEmpty())
    {
        return false;
    }

    mActiveId = mIdStack.pop();
    requestFolder(mActiveId);
    return true;
}

void ServerItemView::onBrowsRequestFinished()
{
    if (mPendingSoapCall == nullptr || mServerItemModel == nullptr)
    {
        // TODO: add qcwarning.
        return;
    }

    if (mServerItemModel->rowCount(QModelIndex{}) > 0)
    {
        mServerItemModel->clearMediaServerObjects();
    }

    const auto result = mPendingSoapCall->resultAs<UPnPAV::BrowseResponse>();
    for (const auto &mediaServerObject : std::as_const(result->objects()))
    {
        mServerItemModel->insertMediaServerObject(mediaServerObject);
    }
}

} // namespace MediaServer::Plugin
