// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef SCPDARGUMENT_H
#define SCPDARGUMENT_H

#include "blabbyupnpav_export.h"
#include <QExplicitlySharedDataPointer>
#include <QString>

namespace UPnPAV
{

struct SCPDArgumentData;
class BLABBYUPNPAV_EXPORT SCPDArgument final
{
public:
    enum Direction : quint8
    {
        In,
        Out
    };

    SCPDArgument();
    SCPDArgument(QString name, Direction direction, QString relatedStateVariable);

    QString name() const;

    Direction direction() const;

    QString relatedStateVariable() const;

    friend bool operator==(SCPDArgument const &lhs, SCPDArgument const &rhs);
    friend bool operator!=(SCPDArgument const &lhs, SCPDArgument const &rhs);

private:
    QExplicitlySharedDataPointer<SCPDArgumentData> d;
};

struct SCPDArgumentData : public QSharedData
{
    QString mName;
    SCPDArgument::Direction mDirection;
    QString mRelatedStateVariable;

    SCPDArgumentData(QString name, SCPDArgument::Direction direction, QString relatedStateVariable)
        : mName{std::move(name)}
        , mDirection{std::move(direction)}
        , mRelatedStateVariable{std::move(relatedStateVariable)}
    {
    }
};

} // namespace UPnPAV

Q_DECLARE_TYPEINFO(UPnPAV::SCPDArgument, Q_RELOCATABLE_TYPE);

#endif // SCPDARGUMENT_H
