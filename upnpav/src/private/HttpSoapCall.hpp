// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef HTTPSOAPCALL_H
#define HTTPSOAPCALL_H

#include "SoapCall.hpp"
#include "blabbyupnpav_export.h"

#include <QSharedPointer>

class QNetworkReply;

namespace UPnPAV
{

class BLABBYUPNPAV_EXPORT HttpSoapCall final : public SoapCall
{
public:
    HttpSoapCall(const QSharedPointer<QNetworkReply> &reply);
    HttpSoapCall(const QSharedPointer<QNetworkReply> &reply, ServiceControlPointDefinition scpd, SCPDAction action);

    bool hasFinishedSuccesful() const noexcept override;

    QString rawMessage() const noexcept override;

private:
    QSharedPointer<QNetworkReply> m_reply;
};

} // namespace UPnPAV

#endif // HTTPSOAPCALL_H
