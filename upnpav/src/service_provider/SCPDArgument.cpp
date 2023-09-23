// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

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
    if (&lhs == &rhs)
    {
        return true;
    }

    return ((lhs.m_name == rhs.m_name) && (lhs.m_direction == rhs.m_direction) &&
            (lhs.m_relatedStateVariable == rhs.m_relatedStateVariable));
}

bool operator!=(const SCPDArgument &lhs, const SCPDArgument &rhs)
{
    return !(lhs == rhs);
}

} // namespace UPnPAV
