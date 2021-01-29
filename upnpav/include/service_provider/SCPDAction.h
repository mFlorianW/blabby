// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef SCPDACTION_H
#define SCPDACTION_H

#include "SCPDArgument.h"

#include <QString>
#include <QVector>

namespace UPnPAV
{

class UPNP_EXPORT SCPDAction
{
public:
    SCPDAction();

    SCPDAction(const QString &name, const QVector<SCPDArgument> &arguments);

    const QString &name() const;

    const QVector<SCPDArgument> &arguments() const;

    QVector<SCPDArgument> inArguments() const noexcept;

    friend bool operator==(const SCPDAction &lhs, const SCPDAction &rhs);
    friend bool operator!=(const SCPDAction &lhs, const SCPDAction &rhs);

private:
    QString m_name;
    QVector<SCPDArgument> m_arguments;
};

} //namespace UPnPAV

#endif // SCPDACTION_H
