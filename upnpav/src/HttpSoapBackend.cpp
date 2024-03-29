// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "HttpSoapBackend.hpp"
#include "private/HttpSoapCall.hpp"
#include "private/LoggingCategories.hpp"
#include <QDebug>
#include <QNetworkReply>

namespace UPnPAV
{

HttpSoapBackend::HttpSoapBackend()
    : SoapBackend()
{
}

HttpSoapBackend::~HttpSoapBackend() = default;

QSharedPointer<SoapCall> HttpSoapBackend::sendSoapMessage(QString const& url,
                                                          QString const& actionName,
                                                          QString const& serviceType,
                                                          QString const& xmlBody) noexcept
{
    QByteArray soapHeader = QString{serviceType + "#" + actionName}.toUtf8();
    QNetworkRequest networkRequest{url};
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml; charset=\"utf-8\"");
    networkRequest.setRawHeader("SOAPACTION", soapHeader);

    QSharedPointer<QNetworkReply> reply{m_accessManager.post(networkRequest, xmlBody.toUtf8())};

    return QSharedPointer<HttpSoapCall>{new (std::nothrow) HttpSoapCall(reply)};
}

QSharedPointer<SoapCall> HttpSoapBackend::sendSoapMessage(ServiceDescription const& desc,
                                                          ServiceControlPointDefinition& scpd,
                                                          SCPDAction const& action,
                                                          QString& xmlBody) noexcept
{
    QByteArray soapHeader = QString{desc.serviceType() + "#" + action.name()}.toUtf8();
    QNetworkRequest networkRequest{desc.controlUrl()};
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml; charset=\"utf-8\"");
    networkRequest.setRawHeader("SOAPACTION", soapHeader);

    qCDebug(upnpavSoapHttp) << networkRequest.url();
    qCDebug(upnpavSoapHttp) << networkRequest.rawHeaderList();
    qCDebug(upnpavSoapHttp) << networkRequest.header(QNetworkRequest::ContentTypeHeader).toString();
    qCDebug(upnpavSoapHttp) << networkRequest.rawHeader("SOAPACTION");
    qCDebug(upnpavSoapHttp).nospace().noquote() << xmlBody;

    QSharedPointer<QNetworkReply> reply{m_accessManager.post(networkRequest, xmlBody.toUtf8())};

    return QSharedPointer<HttpSoapCall>{new (std::nothrow) HttpSoapCall(reply, scpd, action)};
}

} // namespace UPnPAV
