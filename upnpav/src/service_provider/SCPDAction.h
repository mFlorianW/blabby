// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef SCPDACTION_H
#define SCPDACTION_H

#include "SCPDArgument.h"
#include "blabbyupnpav_export.h"
#include <QString>
#include <QVector>

namespace UPnPAV
{

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
    QString m_name;
    QVector<SCPDArgument> m_arguments;
};

} // namespace UPnPAV

#endif // SCPDACTION_H
