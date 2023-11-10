// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include <QObject>

namespace UPnPAV
{

class GetDeviceCapabilitiesResponseShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~GetDeviceCapabilitiesResponseShould() override;
    Q_DISABLE_COPY_MOVE(GetDeviceCapabilitiesResponseShould)
private Q_SLOTS:
    void give_the_play_media_of_the_response();
    void give_the_rec_media_of_the_response();
    void give_the_rec_quality_modes_of_the_response();
};

} // namespace UPnPAV
