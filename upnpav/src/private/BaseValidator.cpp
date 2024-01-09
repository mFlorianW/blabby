// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "BaseValidator.hpp"

namespace UPnPAV
{

BaseValidator::~BaseValidator() = default;

QString const& UPnPAV::BaseValidator::errorMessage() const noexcept
{
    return m_errorMessage;
}

} // namespace UPnPAV
