// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "SCPDArgument.hpp"

namespace UPnPAV
{

SCPDArgument::SCPDArgument() = default;

SCPDArgument::SCPDArgument(QString name, Direction direction, QString relatedStateVariable)
    : d{new SCPDArgumentData{std::move(name), std::move(direction), std::move(relatedStateVariable)}}
{
}

QString SCPDArgument::name() const
{
    return d->mName;
}

SCPDArgument::Direction SCPDArgument::direction() const
{
    return d->mDirection;
}

QString SCPDArgument::relatedStateVariable() const
{
    return d->mRelatedStateVariable;
}

bool operator==(const SCPDArgument &lhs, const SCPDArgument &rhs)
{
    if (&lhs == &rhs) {
        return true;
    }

    return ((lhs.d->mName == rhs.d->mName) && (lhs.d->mDirection == rhs.d->mDirection) &&
            (lhs.d->mRelatedStateVariable == rhs.d->mRelatedStateVariable));
}

bool operator!=(const SCPDArgument &lhs, const SCPDArgument &rhs)
{
    return !(lhs == rhs);
}

} // namespace UPnPAV
