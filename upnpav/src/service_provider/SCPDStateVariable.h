// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef SCPDSTATEVARIABLE_H
#define SCPDSTATEVARIABLE_H

#include "blabbyupnpav_export.h"
#include <QString>
#include <QVector>

namespace UPnPAV
{

class BLABBYUPNPAV_EXPORT SCPDStateVariable
{
public:
    enum DataType
    {
        Unknown,

        Ui1,
        Ui2,
        Ui4,

        I1,
        I2,
        I4,
        // int TODO: handle this type

        R4,
        R8,
        // number TODO: handle this type
        // fixed TODO: handle this type
        // float TODO: TODO: handle this type

        Char,
        String,

        Date,
        DateTime,
        DateTimeTz,
        Time,
        TimeTz,

        Bool,

        // Bin.base64 TODO: handle this type
        // Bin.hex TODO: handle this type

        Uri,
        Uuid,
    };

    SCPDStateVariable();

    SCPDStateVariable(bool m_sendEvents,
                      const QString &name,
                      DataType dataType,
                      const QString &defaultValue = {},
                      const QVector<QString> &allowedValues = {},
                      const QString &miniumValue = {},
                      const QString &maximumValue = {},
                      const QString &step = {});

    bool sendEvents() const;

    const QString &name() const;

    DataType dataType() const;

    const QString &defaultValue() const;

    const QVector<QString> &allowedValues() const;

    const QString &miniumValue() const;

    const QString &maximumValue() const;

    const QString &stepSize() const;

    BLABBYUPNPAV_EXPORT friend bool operator==(const SCPDStateVariable &lhs, const SCPDStateVariable &rhs);
    BLABBYUPNPAV_EXPORT friend bool operator!=(const SCPDStateVariable &lhs, const SCPDStateVariable &rhs);

private:
    bool m_sendEvents{false};
    QString m_name;
    DataType m_dataType;
    QString m_defaultValue;
    QVector<QString> m_allowedValues;
    QString m_miniumValue;
    QString m_maximumValue;
    QString m_step;
};

} // namespace UPnPAV

#endif // SCPDSTATEVARIABLE_H
