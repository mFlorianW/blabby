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
#include "SCPDStateVariable.h"

namespace UPnPAV
{

SCPDStateVariable::SCPDStateVariable()
{
}

SCPDStateVariable::SCPDStateVariable(bool sendEvents,
                                     const QString &name,
                                     DataType dataType,
                                     const QString &defaultValue,
                                     const QVector<QString> &allowedValues,
                                     const QString &miniumValue,
                                     const QString &maximumValue,
                                     const QString &step)
    : m_sendEvents(sendEvents)
    , m_name(name)
    , m_dataType(dataType)
    , m_defaultValue(defaultValue)
    , m_allowedValues(allowedValues)
    , m_miniumValue(miniumValue)
    , m_maximumValue(maximumValue)
    , m_step(step)
{
}

bool SCPDStateVariable::sendEvents() const
{
    return m_sendEvents;
}

const QString &SCPDStateVariable::name() const
{
    return m_name;
}

const QString &SCPDStateVariable::defaultValue() const
{
    return m_defaultValue;
}

const QVector<QString> &SCPDStateVariable::allowedValues() const
{
    return m_allowedValues;
}

const QString &SCPDStateVariable::miniumValue() const
{
    return m_miniumValue;
}

const QString &SCPDStateVariable::maximumValue() const
{
    return m_maximumValue;
}

const QString &SCPDStateVariable::stepSize() const
{
    return m_step;
}

bool operator==(const SCPDStateVariable &lhs, const SCPDStateVariable &rhs)
{
    if(&lhs == &rhs)
    {
        return true;
    }

    return ((lhs.m_sendEvents == rhs.m_sendEvents) &&
            (lhs.m_name == rhs.m_name) &&
            (lhs.m_dataType == rhs.m_dataType) &&
            (lhs.m_defaultValue == rhs.m_defaultValue) &&
            (lhs.m_allowedValues == rhs.m_allowedValues) &&
            (lhs.m_miniumValue == rhs.m_miniumValue) &&
            (lhs.m_maximumValue == rhs.m_maximumValue) &&
            (lhs.m_step == rhs.m_step));
}

bool operator!=(const SCPDStateVariable &lhs, const SCPDStateVariable &rhs)
{
    return !(lhs == rhs);
}

SCPDStateVariable::DataType SCPDStateVariable::dataType() const
{
    return m_dataType;
}

} //namespace UPnPAV
