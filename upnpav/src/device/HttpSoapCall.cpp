// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "HttpSoapCall.h"

#include <QNetworkReply>

namespace UPnPAV
{

HttpSoapCall::HttpSoapCall(const QSharedPointer<QNetworkReply> &reply)
    : SoapCall()
    , m_reply(reply)
{
    (void)connect(m_reply.get(),
                  &QNetworkReply::finished,
                  this,
                  &HttpSoapCall::finished);
}

bool HttpSoapCall::hasFinishedSuccesful() const noexcept
{
    return (m_reply->error() == QNetworkReply::NoError) ? true
                                                        : false;
}

QString HttpSoapCall::rawMessage() const noexcept
{
    return QString{m_reply->readAll()};
}

} //namespace UPnPAV
