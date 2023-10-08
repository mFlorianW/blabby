// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef SOAPMESSAGEGENERATOR_H
#define SOAPMESSAGEGENERATOR_H

#include "blabbyupnpav_export.h"
#include <QMap>
#include <QString>

namespace UPnPAV
{
class SCPDAction;
struct Argument
{
    QString name;
    QString value;
};

using ArgumentList = QVector<Argument>;

class BLABBYUPNPAV_EXPORT SoapMessageGenerator
{
public:
    SoapMessageGenerator();

    QString generateXmlMessageBody(const SCPDAction &action,
                                   const QString &serviceType,
                                   ArgumentList args = {}) noexcept;

private:
    static QVector<Argument> putArgumentsInOrderAsInDefiniton(const QVector<Argument> &args, const SCPDAction &action);
};

} // namespace UPnPAV

#endif // SOAPMESSAGEGENERATOR_H
