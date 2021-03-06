// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef SOAPCALLDOUBLE_H
#define SOAPCALLDOUBLE_H

#include <SoapCall.h>

namespace UPnPAV
{

class SoapCallDouble : public SoapCall
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

} //namespace UPnPAV

#endif // SOAPCALLDOUBLE_H
