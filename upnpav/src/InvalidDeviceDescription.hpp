// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef INVALIDDEVICEDESCRIPTION_H
#define INVALIDDEVICEDESCRIPTION_H

#include "blabbyupnpav_export.h"
#include <QException>

namespace UPnPAV
{

class BLABBYUPNPAV_EXPORT InvalidDeviceDescription : public QException
{
public:
    InvalidDeviceDescription(QString const& errorMessage);

    char const* what() const noexcept override;

    void raise() const override;

    InvalidDeviceDescription* clone() const override;

private:
    QByteArray m_errorMessage;
};

} // namespace UPnPAV

#endif // INVALIDDEVICEDESCRIPTION_H
