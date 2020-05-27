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
#ifndef HTTPSOAPCALL_H
#define HTTPSOAPCALL_H

#include "UPnP_Export.h"
#include "SoapCall.h"

#include <QSharedPointer>

class QNetworkReply;

namespace UPnPAV
{

class UPNP_EXPORT HttpSoapCall final : public SoapCall
{
public:
    HttpSoapCall(const QSharedPointer<QNetworkReply> &reply);

    bool hasFinishedSuccesful() const noexcept override;

    QString rawMessage() const noexcept override;

private:
    QSharedPointer<QNetworkReply> m_reply;
};

} //namespace UPnPAV

#endif // HTTPSOAPCALL_H
