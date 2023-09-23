// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef SOAPCALLDOUBLE_H
#define SOAPCALLDOUBLE_H

#include "SoapCall.h"

namespace MediaServer::Plugin::Doubles
{

class SoapCallDouble : public UPnPAV::SoapCall
{
public:
    SoapCallDouble();

    void setErrorState(bool error);
    bool hasFinishedSuccesful() const noexcept override;

    void setRawMessage(const QString &rawMessage);
    QString rawMessage() const noexcept override;

private:
    bool m_errorState;
    QString m_rawMessage;
};

} // namespace MediaServer::Plugin::Doubles

#endif // SOAPCALLDOUBLE_H
