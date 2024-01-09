// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "SCPDAction.hpp"

#include <utility>

namespace UPnPAV
{

SCPDAction::SCPDAction()
    : d{new SCPDActionData{QString{""}, {}}}
{
}

SCPDAction::SCPDAction(QString name, QVector<SCPDArgument> arguments)
    : d{new SCPDActionData{std::move(name), std::move(arguments)}}
{
}

QString const& SCPDAction::name() const
{
    return d->mName;
}

QVector<SCPDArgument> const& SCPDAction::arguments() const
{
    return d->mArguments;
}

QVector<SCPDArgument> SCPDAction::inArguments() const noexcept
{
    QVector<SCPDArgument> result;
    for (auto const& arg : std::as_const(d->mArguments)) {
        if (arg.direction() == SCPDArgument::In) {
            result.append(arg);
        }
    }

    return result;
}

QVector<SCPDArgument> SCPDAction::outArguments() const noexcept
{
    QVector<SCPDArgument> result;
    for (auto const& arg : std::as_const(d->mArguments)) {
        if (arg.direction() == SCPDArgument::Out) {
            result.append(arg);
        }
    }
    return result;
}

bool operator==(SCPDAction const& lhs, SCPDAction const& rhs)
{
    if (&lhs == &rhs) {
        return true;
    }

    return ((lhs.d->mName == rhs.d->mName) && (lhs.d->mArguments == rhs.d->mArguments));
}

bool operator!=(SCPDAction const& lhs, SCPDAction const& rhs)
{
    return !(lhs == rhs);
}

} // namespace UPnPAV
