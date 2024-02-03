// Copyright 2019 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "ServiceControlPointDefinition.hpp"
#include "ServiceDescription.hpp"
#include "SoapCall.hpp"
#include "blabbyupnpav_export.h"
#include <QSharedPointer>

class QString;

namespace UPnPAV
{

class BLABBYUPNPAV_EXPORT SoapBackend
{
public:
    virtual ~SoapBackend();

    Q_DISABLE_COPY_MOVE(SoapBackend)

    virtual QSharedPointer<SoapCall> sendSoapMessage(QString const& url,
                                                     QString const& actionName,
                                                     QString const& serviceType,
                                                     QString const& xmlBody) noexcept = 0;

    virtual QSharedPointer<SoapCall> sendSoapMessage(ServiceDescription const& desc,
                                                     ServiceControlPointDefinition& scpd,
                                                     SCPDAction const& action,
                                                     QString& xmlBody) noexcept = 0;

protected:
    SoapBackend() = default;
};

} // namespace UPnPAV
