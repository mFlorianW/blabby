// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "HttpSoapMessageTransmitter.h"
#include "HttpSoapCall.h"

#include <QNetworkReply>
#include <QDebug>

namespace UPnPAV
{

HttpSoapMessageTransmitter::HttpSoapMessageTransmitter()
    : SoapMessageTransmitter()
{
}

HttpSoapMessageTransmitter::~HttpSoapMessageTransmitter()
{
}

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

    QSharedPointer<QNetworkReply> reply
    {
        m_accessManager.post(networkRequest, xmlBody.toUtf8())
    };

    return QSharedPointer<HttpSoapCall>
    {
        new HttpSoapCall(reply)
    };
}

} //namespace UPnPAV
