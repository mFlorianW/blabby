
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include <QObject>

namespace UPnPAV
{
class GetTransportInfoResponseShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    Q_DISABLE_COPY_MOVE(GetTransportInfoResponseShould)
    ~GetTransportInfoResponseShould() override;
private Q_SLOTS:
    void give_the_current_transport_state_of_a_response();
    void give_the_current_transport_status_of_a_response();
    void give_the_transport_play_speed_of_a_response();
};
} // namespace UPnPAV
