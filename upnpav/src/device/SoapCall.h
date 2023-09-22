// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef SOAPCALL_H
#define SOAPCALL_H

#include "blabbyupnpav_export.h"
#include <QObject>

namespace UPnPAV
{

class BLABBYUPNPAV_EXPORT SoapCall : public QObject
{
    Q_OBJECT
public:
    virtual ~SoapCall();

    /**
     * Status of the SOAP call. True means call finished successful otherwise false.
     *
     * @note The return value is only valid when finished signal was emitted.
     *
     * @return True call finished successful, otherwise false.
     */
    virtual bool hasFinishedSuccesful() const noexcept = 0;

    /**
     * Raw answer of the error code.
     *
     * @return The raw response of the Soap call.
     */
    virtual QString rawMessage() const noexcept = 0;

Q_SIGNALS:
    /**
     * This signal shall be emitted when the Soap call
     * is finished.
     */
    void finished();
};

} // namespace UPnPAV

#endif // SOAPCALL_H
