// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <QObject>

namespace UPnPAV
{
class GetCurrentConnectionIDsResponseShould : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(GetCurrentConnectionIDsResponseShould)
public:
    using QObject::QObject;
    ~GetCurrentConnectionIDsResponseShould() override = default;

private Q_SLOTS:
    void give_the_correct_ids_from_response() noexcept;
};

} // namespace UPnPAV
