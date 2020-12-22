#include "ServerItemController.h"
#include "BrowseResult.h"
#include "IMediaServer.h"
#include "PendingSoapCall.h"
#include "ServerItemModel.h"

namespace MediaServerPlugin
{

ServerItemController::ServerItemController()
{
}

void MediaServerPlugin::ServerItemController::setMediaServer(UPnPAV::IMediaServer *mediaServer) noexcept
{
    mMediaServer = mediaServer;

    if(mediaServer != nullptr)
    {
        mPendingSoapCall = mediaServer->browse("0", UPnPAV::IMediaServer::DirectChildren, "", "");
        connect(mPendingSoapCall.get(), &UPnPAV::PendingSoapCall::finished, this, &ServerItemController::onBrowsRequestFinished);
    }
}

void ServerItemController::setServerItemModel(ServerItemModel *serverItemModel) noexcept
{
    mServerItemModel = serverItemModel;
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
    mServerItemModel->insertMediaServerObject(result->objects().first());
}

} // namespace MediaServerPlugin
