// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef SCPDARGUMENT_H
#define SCPDARGUMENT_H

#include "blabbyupnpav_export.h"
#include <QString>

namespace UPnPAV
{

class BLABBYUPNPAV_EXPORT SCPDArgument final
{
public:
    enum Direction : quint8
    {
        In,
        Out
    };

    SCPDArgument();
    SCPDArgument(const QString &name, Direction direction, const QString &relatedStateVariable);

    QString name() const;

    Direction direction() const;

    QString relatedStateVariable() const;

    friend bool operator==(const SCPDArgument &lhs, const SCPDArgument &rhs);
    friend bool operator!=(const SCPDArgument &lhs, const SCPDArgument &rhs);

private:
    QString m_name;
    Direction m_direction;
    QString m_relatedStateVariable;
};

} // namespace UPnPAV

#endif // SCPDARGUMENT_H
