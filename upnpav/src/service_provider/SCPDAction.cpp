// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

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
    for (const auto &arg : m_arguments)
    {
        if (arg.direction() == SCPDArgument::In)
        {
            result.append(arg);
        }
    }

    return result;
}

bool operator==(const SCPDAction &lhs, const SCPDAction &rhs)
{
    if (&lhs == &rhs)
    {
        return true;
    }

    return ((lhs.m_name == rhs.m_name) && (lhs.m_arguments == rhs.m_arguments));
}

bool operator!=(const SCPDAction &lhs, const SCPDAction &rhs)
{
    return !(lhs == rhs);
}

} // namespace UPnPAV
