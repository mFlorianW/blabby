/**
 ** This file is part of the Blabby project.
 ** Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as
 ** published by the Free Software Foundation, either version 2 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
#ifndef SCPDSTATEVARIABLE_H
#define SCPDSTATEVARIABLE_H

#include "UPnP_Export.h"

#include <QString>
#include <QVector>

namespace UPnPAV
{

class UPNP_EXPORT SCPDStateVariable
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
        //int TODO: handle this type

        R4,
        R8,
        //number TODO: handle this type
        //fixed TODO: handle this type
        //float TODO: TODO: handle this type

        Char,
        String,

        Date,
        DateTime,
        DateTimeTz,
        Time,
        TimeTz,

        Bool,

        //Bin.base64 TODO: handle this type
        //Bin.hex TODO: handle this type

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

    friend bool operator==(const SCPDStateVariable &lhs, const SCPDStateVariable &rhs);
    friend bool operator!=(const SCPDStateVariable &lhs, const SCPDStateVariable &rhs);

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

} //namespace UPnPAV

#endif // SCPDSTATEVARIABLE_H
