// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "HttpEventBackend.hpp"
#include "private/HttpEventServer.hpp"
#include "private/HttpEventSubscriptionHandle.hpp"
#include <QUrl>

namespace UPnPAV
{

HttpEventBackend::HttpEventBackend()
    : mEventServer{HttpEventServer::instance()}
{
}

HttpEventBackend::~HttpEventBackend() = default;

QUrl HttpEventBackend::callbackHost() const noexcept
{
    return mEventServer.callbackHost();
}

QString const& HttpEventBackend::registerEventCallback(ServiceDescription const& desc) noexcept
{
    Q_UNUSED(desc)
    return mEmptyString;
}

std::shared_ptr<EventSubscriptionHandle> HttpEventBackend::sendSubscriptionRequest(
    EventSubscriptionParameters const& params) noexcept
{
    auto handle = std::make_shared<HttpEventSubscriptionHandle>(mEventServer.serverAddress());
    handle->subscribe(params);
    return handle;
}

} // namespace UPnPAV
