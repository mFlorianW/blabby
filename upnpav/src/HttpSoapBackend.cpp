// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "HttpSoapBackend.hpp"
#include "private/HttpSoapCall.hpp"
#include "private/LoggingCategories.hpp"
#include <QDebug>
#include <QNetworkReply>

namespace UPnPAV
{

namespace
{
/**
 * Gives the value of the SOAPACTION header, the action in double quotes as the UPnP Device Architecture demands.
 * Some devices, e.g. Samsung TVs, reject a call without the quotes with "Invalid Args".
 */
QByteArray soapActionHeader(QString const& serviceType, QString const& actionName)
{
    return QStringLiteral("\"%1#%2\"").arg(serviceType, actionName).toUtf8();
}
} // namespace

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
    auto const soapHeader = soapActionHeader(serviceType, actionName);
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
    auto const soapHeader = soapActionHeader(desc.serviceType(), action.name());
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
