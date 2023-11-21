// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "SCPDAction.hpp"

#include <utility>

namespace UPnPAV
{

SCPDAction::SCPDAction() = default;

SCPDAction::SCPDAction(QString name, QVector<SCPDArgument> arguments)
    : d{new SCPDActionData{std::move(name), std::move(arguments)}}
{
}

const QString &SCPDAction::name() const
{
    return d->mName;
}

const QVector<SCPDArgument> &SCPDAction::arguments() const
{
    return d->mArguments;
}

QVector<SCPDArgument> SCPDAction::inArguments() const noexcept
{
    QVector<SCPDArgument> result;
    for (const auto &arg : std::as_const(d->mArguments))
    {
        if (arg.direction() == SCPDArgument::In)
        {
            result.append(arg);
        }
    }

    return result;
}

QVector<SCPDArgument> SCPDAction::outArguments() const noexcept
{
    QVector<SCPDArgument> result;
    for (auto const &arg : std::as_const(d->mArguments))
    {
        if (arg.direction() == SCPDArgument::Out)
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

    return ((lhs.d->mName == rhs.d->mName) && (lhs.d->mArguments == rhs.d->mArguments));
}

bool operator!=(const SCPDAction &lhs, const SCPDAction &rhs)
{
    return !(lhs == rhs);
}

} // namespace UPnPAV
