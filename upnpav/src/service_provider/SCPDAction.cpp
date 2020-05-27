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
#include "SCPDAction.h"

namespace UPnPAV
{


SCPDAction::SCPDAction()
{
}

SCPDAction::SCPDAction(const QString &name, const QVector<SCPDArgument> &arguments)
    : m_name(name)
    , m_arguments(arguments)
{
}

const QString &SCPDAction::name() const
{
    return m_name;
}

const QVector<SCPDArgument> &SCPDAction::arguments() const
{
    return m_arguments;
}

QVector<SCPDArgument> SCPDAction::inArguments() const noexcept
{
    QVector<SCPDArgument> result;
    for(const auto &arg : m_arguments)
    {
        if(arg.direction() == SCPDArgument::In)
        {
            result.append(arg);
        }
    }

    return result;
}

bool operator==(const SCPDAction &lhs, const SCPDAction &rhs)
{
    if(&lhs == &rhs)
    {
        return true;
    }

    return ((lhs.m_name == rhs.m_name) &&
            (lhs.m_arguments == rhs.m_arguments));
}

bool operator!=(const SCPDAction &lhs, const SCPDAction &rhs)
{
    return !(lhs == rhs);
}

} // namespace UPnPAV
