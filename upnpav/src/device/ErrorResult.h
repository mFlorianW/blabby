// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef ERRORRESULT_H
#define ERRORRESULT_H

#include <QString>

namespace UPnPAV
{

class ErrorResult
{
public:
    ErrorResult(const QString &rawMessage);

    qint32 errorCode() const noexcept;

    QString errorDescription() const noexcept;

private:
    qint32 m_errorCode{0};
    QString m_errorDescription;
};

} //namespace UPnPAV

#endif // ERRORRESULT_H
