// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <QObject>

namespace UPnPAV
{

class GetPositionInfoResponseShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~GetPositionInfoResponseShould() override = default;
    Q_DISABLE_COPY_MOVE(GetPositionInfoResponseShould)

private Q_SLOTS:
    void give_the_track_of_the_response();
    void give_the_track_duration_of_the_response();
    void give_the_track_meta_data_of_the_response();
    void give_the_track_uri_of_the_response();
    void give_the_rel_time_of_the_response();
    void give_the_abs_time_of_the_response();
    void give_the_rel_count_of_the_response();
    void give_the_abs_count_of_the_response();
};

} // namespace UPnPAV
