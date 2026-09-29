// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <QObject>

namespace UPnPAV
{

class GetMuteResponseShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~GetMuteResponseShould() override;
    Q_DISABLE_COPY_MOVE(GetMuteResponseShould)

private Q_SLOTS:
    void give_the_mute_of_the_response_data();
    void give_the_mute_of_the_response();
};

} // namespace UPnPAV
