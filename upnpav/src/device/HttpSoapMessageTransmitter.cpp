/**
 ** This file is part of the Blabby project.
 ** Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as
 ** published by the Free Software Foundation, either version 2 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
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
