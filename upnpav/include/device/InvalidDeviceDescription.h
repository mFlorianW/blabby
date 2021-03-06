// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef INVALIDDEVICEDESCRIPTION_H
#define INVALIDDEVICEDESCRIPTION_H

#include "UPnP_Export.h"

#include <QException>

namespace UPnPAV
{

class UPNP_EXPORT InvalidDeviceDescription : public QException
{
public:
    InvalidDeviceDescription(const QString &errorMessage);

    const char *what() const noexcept override;

    void raise() const override;

    InvalidDeviceDescription *clone() const override;

private:
    QByteArray m_errorMessage;
};

} //namespace UPnPAV

#endif // INVALIDDEVICEDESCRIPTION_H
