// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once
#include <QObject>

namespace UPnPAV
{

class ProtocolInfoResultShould final : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(ProtocolInfoResultShould)
public:
    using QObject::QObject;
    ~ProtocolInfoResultShould() override = default;

private Q_SLOTS:
    void give_the_protocol_lists();
};
} // namespace UPnPAV
