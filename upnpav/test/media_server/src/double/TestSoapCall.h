// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef TESTSOAPCALL_H
#define TESTSOAPCALL_H

#include "SoapCall.h"

namespace UPnPAV
{

class TestSoapCall : public SoapCall
{
    Q_OBJECT
public:
    bool hasFinishedSuccesful() const noexcept override;
    QString rawMessage() const noexcept override;
};

} //namespace UPnPAV

#endif // TESTSOAPCALL_H
