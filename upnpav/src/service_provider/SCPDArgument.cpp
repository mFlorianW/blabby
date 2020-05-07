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
#include "SCPDArgument.h"

namespace UPnPAV
{

SCPDArgument::SCPDArgument()
{
}

SCPDArgument::SCPDArgument(const QString &name,
                           UPnPAV::SCPDArgument::Direction direction,
                           const QString &relatedStateVariable)
    : m_name(name)
    , m_direction(direction)
    , m_relatedStateVariable(relatedStateVariable)
{
}

QString SCPDArgument::name() const
{
    return m_name;
}

SCPDArgument::Direction SCPDArgument::direction() const
{
    return m_direction;
}

QString SCPDArgument::relatedStateVariable() const
{
    return m_relatedStateVariable;
}

bool operator==(const SCPDArgument &lhs, const SCPDArgument &rhs)
{
    if(&lhs == &rhs)
    {
        return true;
    }

    return ((lhs.m_name == rhs.m_name) &&
            (lhs.m_direction == rhs.m_direction) &&
            (lhs.m_relatedStateVariable == rhs.m_relatedStateVariable));
}

bool operator!=(const SCPDArgument &lhs, const SCPDArgument &rhs)
{
    return !(lhs == rhs);
}

} //namespace UPnPAV

