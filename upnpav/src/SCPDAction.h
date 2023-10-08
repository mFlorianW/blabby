// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef SCPDACTION_H
#define SCPDACTION_H

#include "SCPDArgument.h"
#include "blabbyupnpav_export.h"
#include <QExplicitlySharedDataPointer>
#include <QString>
#include <QVector>

namespace UPnPAV
{

struct SCPDActionData;
class BLABBYUPNPAV_EXPORT SCPDAction
{
public:
    SCPDAction();

    SCPDAction(QString name, QVector<SCPDArgument> arguments);

    const QString &name() const;

    const QVector<SCPDArgument> &arguments() const;

    QVector<SCPDArgument> inArguments() const noexcept;

    QVector<SCPDArgument> outArguments() const noexcept;

    BLABBYUPNPAV_EXPORT friend bool operator==(const SCPDAction &lhs, const SCPDAction &rhs);
    BLABBYUPNPAV_EXPORT friend bool operator!=(const SCPDAction &lhs, const SCPDAction &rhs);

private:
    QExplicitlySharedDataPointer<SCPDActionData> d;
};

struct SCPDActionData : public QSharedData
{
    QString mName;
    QVector<SCPDArgument> mArguments;

    SCPDActionData(QString name, QVector<SCPDArgument> arguments)
        : mName(std::move(name))
        , mArguments(std::move(arguments))
    {
    }
};

} // namespace UPnPAV

Q_DECLARE_TYPEINFO(UPnPAV::SCPDActionData, Q_RELOCATABLE_TYPE);

#endif // SCPDACTION_H
