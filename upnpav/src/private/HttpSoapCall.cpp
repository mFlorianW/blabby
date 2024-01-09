// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "HttpSoapCall.hpp"

#include <QNetworkReply>

namespace UPnPAV
{

HttpSoapCall::HttpSoapCall(QSharedPointer<QNetworkReply> const& reply)
    : SoapCall()
    , m_reply(reply)
{
    (void)connect(m_reply.get(), &QNetworkReply::finished, this, &HttpSoapCall::finished);
}

HttpSoapCall::HttpSoapCall(QSharedPointer<QNetworkReply> const& reply,
                           ServiceControlPointDefinition scpd,
                           SCPDAction action)
    : SoapCall(std::move(scpd), std::move(action))
    , m_reply(reply)
{
    (void)connect(m_reply.get(), &QNetworkReply::finished, this, &HttpSoapCall::finished);
}

bool HttpSoapCall::hasFinishedSuccesful() const noexcept
{
    return (m_reply->error() == QNetworkReply::NoError) ? true : false;
}

QString HttpSoapCall::rawMessage() const noexcept
{
    return QString{m_reply->readAll()};
}

} // namespace UPnPAV
