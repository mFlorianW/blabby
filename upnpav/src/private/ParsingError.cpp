// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ParsingError.hpp"

#include <utility>

ParsingError::ParsingError(QString errorDescription)
    : m_errorDescription(std::move(errorDescription).toLocal8Bit())
{
}

char const* ParsingError::what() const noexcept
{
    return m_errorDescription.data();
}

void ParsingError::raise() const
{
    throw *this;
}

ParsingError* ParsingError::clone() const
{
    return new ParsingError{*this}; // NOLINT cppcoreguidelines-owning-memory
}
