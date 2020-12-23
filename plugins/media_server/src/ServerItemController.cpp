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
        mPendingSoapCall = mediaServer->browse("0", UPnPAV::IMediaServer::DirectChildren, "", "");
        connect(mPendingSoapCall.get(), &UPnPAV::PendingSoapCall::finished, this, &ServerItemController::onBrowsRequestFinished);
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

void ServerItemController::onBrowsRequestFinished()
{
    if(mPendingSoapCall == nullptr)
    {
        // TODO: add qcwarning.
        return;
    }

    if(mServerItemModel == nullptr)
    {
        // TODO: add qcwarning;
        return;
    }

    const auto result = mPendingSoapCall->resultAs<UPnPAV::BrowseResult>();
    for(const auto &mediaServerObject : std::as_const(result->objects()))
    {
        mServerItemModel->insertMediaServerObject(mediaServerObject);
    }
}

} // namespace MediaServer::Plugin
