// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef SOAPCALLDOUBLE_H
#define SOAPCALLDOUBLE_H

#include "SoapCall.hpp"
#include <qdebug.h>

namespace UPnPAV
{

class SoapCallDouble : public SoapCall
{
    Q_OBJECT
public:
    SoapCallDouble();
    SoapCallDouble(ServiceControlPointDefinition scpd, SCPDAction action);

    ~SoapCallDouble() override;

    Q_DISABLE_COPY_MOVE(SoapCallDouble)

    void setErrorState(bool error);
    bool hasFinishedSuccesful() const noexcept override;

    void setRawMessage(QString const &rawMessage);
    QString rawMessage() const noexcept override;

private:
    bool m_errorState{false};
    QString m_rawMessage{""};
};

} // namespace UPnPAV

#endif // SOAPCALLDOUBLE_H
