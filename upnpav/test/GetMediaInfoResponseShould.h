// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include <QObject>

namespace UPnPAV
{

class GetMediaInfoResponseShould : public QObject
{
    Q_OBJECT
public:
    Q_DISABLE_COPY_MOVE(GetMediaInfoResponseShould)

    using QObject::QObject;

    ~GetMediaInfoResponseShould() override;

private Q_SLOTS:
    void the_get_media_response_shall_return_the_nr_tracks_of_a_response();
    void give_the_media_duration_of_a_response();
    void give_the_current_uri_of_a_response();
    void give_the_current_meta_data_of_a_response();
    void give_the_next_uri_of_a_response();
    void give_the_next_uri_meta_data_of_a_response();
    void give_the_play_medium_of_a_response();
    void give_the_record_medium_of_a_response();
    void give_the_write_status_of_a_response();
};

} // namespace UPnPAV
