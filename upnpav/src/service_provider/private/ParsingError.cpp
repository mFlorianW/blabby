// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ParsingError.h"

ParsingError::ParsingError(const QString &errorDescription)
    : m_errorDescription(errorDescription)
{
}

const char *ParsingError::what() const noexcept
{
    return m_errorDescription.toUtf8().data();
}

void ParsingError::raise() const
{
    throw *this;
}

ParsingError *ParsingError::clone() const
{
    return new ParsingError{*this};
}
