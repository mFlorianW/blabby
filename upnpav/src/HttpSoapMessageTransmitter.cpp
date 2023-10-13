// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "HttpSoapMessageTransmitter.h"
#include "private/HttpSoapCall.h"
#include <QDebug>
#include <QNetworkReply>

namespace UPnPAV
{

HttpSoapMessageTransmitter::HttpSoapMessageTransmitter()
    : SoapMessageTransmitter()
{
}

HttpSoapMessageTransmitter::~HttpSoapMessageTransmitter() = default;

QSharedPointer<SoapCall> HttpSoapMessageTransmitter::sendSoapMessage(const QString &url,
                                                                     const QString &actionName,
                                                                     const QString &serviceType,
                                                                     const QString &xmlBody) noexcept
{
    QByteArray soapHeader = QString{serviceType + "#" + actionName}.toUtf8();
    QNetworkRequest networkRequest{url};
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml; charset=\"utf-8\"");
    networkRequest.setRawHeader("SOAPACTION", soapHeader);

    qDebug() << networkRequest.url();
    qDebug() << networkRequest.rawHeaderList();
    qDebug() << networkRequest.header(QNetworkRequest::ContentTypeHeader).toString();
    qDebug() << networkRequest.rawHeader("SOAPACTION");

    QSharedPointer<QNetworkReply> reply{m_accessManager.post(networkRequest, xmlBody.toUtf8())};

    return QSharedPointer<HttpSoapCall>{new (std::nothrow) HttpSoapCall(reply)};
}

QSharedPointer<SoapCall> HttpSoapMessageTransmitter::sendSoapMessage(ServiceDescription const &desc,
                                                                     ServiceControlPointDefinition &scpd,
                                                                     SCPDAction const &action,
                                                                     QString &xmlBody) noexcept
{
    QByteArray soapHeader = QString{desc.serviceType() + "#" + action.name()}.toUtf8();
    QNetworkRequest networkRequest{desc.controlUrl()};
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml; charset=\"utf-8\"");
    networkRequest.setRawHeader("SOAPACTION", soapHeader);

    qDebug() << networkRequest.url();
    qDebug() << networkRequest.rawHeaderList();
    qDebug() << networkRequest.header(QNetworkRequest::ContentTypeHeader).toString();
    qDebug() << networkRequest.rawHeader("SOAPACTION");

    QSharedPointer<QNetworkReply> reply{m_accessManager.post(networkRequest, xmlBody.toUtf8())};

    return QSharedPointer<HttpSoapCall>{new (std::nothrow) HttpSoapCall(reply, scpd, action)};
}

} // namespace UPnPAV