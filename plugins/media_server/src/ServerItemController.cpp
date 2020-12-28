#include "ServerItemController.h"
#include "BrowseResult.h"
#include "IMediaServer.h"
#include "PendingSoapCall.h"
#include "ServerItemModel.h"

namespace MediaServer::Plugin
{

ServerItemController::ServerItemController()
    : QQuickItem()
{
}

ServerItemController::~ServerItemController() = default;

UPnPAV::IMediaServer *ServerItemController::mediaServer() const noexcept
{
    return mMediaServer;
}

void ServerItemController::setMediaServer(UPnPAV::IMediaServer *mediaServer) noexcept
{
    mMediaServer = mediaServer;

    if(mediaServer != nullptr)
    {
        requestFolder("0");
    }

    Q_EMIT mediaServerChanged();
}

ServerItemModel *ServerItemController::serverItemModel() const noexcept
{
    return mServerItemModel;
}

void ServerItemController::setServerItemModel(ServerItemModel *serverItemModel) noexcept
{
    mServerItemModel = serverItemModel;
    Q_EMIT serverItemModelChanged();
}

void MediaServer::Plugin::ServerItemController::requestFolder(const QString &id)
{
    mPendingSoapCall = mMediaServer->browse(id, UPnPAV::IMediaServer::DirectChildren, "", "");
    connect(mPendingSoapCall.get(), &UPnPAV::PendingSoapCall::finished, this, &ServerItemController::onBrowsRequestFinished);
}

void ServerItemController::requestStorageFolder(const QString &id)
{
    if(mMediaServer == nullptr)
    {
        // TODO: add qcwarning
        return;
    }

    requestFolder(id);
}

void ServerItemController::onBrowsRequestFinished()
{
    if(mPendingSoapCall == nullptr || mServerItemModel == nullptr)
    {
        // TODO: add qcwarning.
        return;
    }

    if(mServerItemModel->rowCount(QModelIndex{}) > 0)
    {
        mServerItemModel->clearMediaServerObjects();
    }

    const auto result = mPendingSoapCall->resultAs<UPnPAV::BrowseResult>();
    for(const auto &mediaServerObject : std::as_const(result->objects()))
    {
        mServerItemModel->insertMediaServerObject(mediaServerObject);
    }
}

} // namespace MediaServer::Plugin
