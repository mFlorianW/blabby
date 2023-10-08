// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef PARSINGERROR_H
#define PARSINGERROR_H

#include "blabbyupnpav_export.h"
#include <QException>

class BLABBYUPNPAV_EXPORT ParsingError : public QException
{
public:
    /**
     *
     * @param errorDescription
     */
    ParsingError(const QString &errorDescription);

    /**
     *
     * @return
     */
    const char *what() const noexcept override;

    /**
     *
     */
    void raise() const override;

    /**
     *
     * @return
     */
    ParsingError *clone() const override;

private:
    QString m_errorDescription;
};

#endif // PARSINGERROR_H
