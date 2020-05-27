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
