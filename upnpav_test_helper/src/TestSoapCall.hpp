// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef TESTSOAPCALL_H
#define TESTSOAPCALL_H

#include "SoapCall.hpp"

namespace UPnPAV
{

class TestSoapCall : public SoapCall
{
    Q_OBJECT
public:
    TestSoapCall();
    /**
     * Creates a call that knows the SCPD and the action it was sent for, like a call of the real SOAP backend.
     */
    TestSoapCall(ServiceControlPointDefinition scpd, SCPDAction action);

    bool hasFinishedSuccesful() const noexcept override;
    bool isDeviceUnreachable() const noexcept override;
    QString rawMessage() const noexcept override;
};

} // namespace UPnPAV

#endif // TESTSOAPCALL_H
