// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include <QObject>
#include <qobject.h>

namespace UPnPAV
{
class CurrentConnectionIDsResultShould : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(CurrentConnectionIDsResultShould)
public:
    using QObject::QObject;
    ~CurrentConnectionIDsResultShould() override = default;

private Q_SLOTS:
    void give_the_correct_ids_from_response() noexcept;
};

} // namespace UPnPAV
