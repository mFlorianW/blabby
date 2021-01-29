// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "BaseValidator.h"

namespace UPnPAV
{

BaseValidator::~BaseValidator()
{
}

const QString &UPnPAV::BaseValidator::errorMessage() const noexcept
{
    return m_errorMessage;
}

} //namespace
