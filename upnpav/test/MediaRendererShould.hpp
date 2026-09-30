// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <QObject>

namespace UPnPAV
{
class MediaRendererShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~MediaRendererShould() override;
    Q_DISABLE_COPY_MOVE(MediaRendererShould)
private Q_SLOTS:
    void throw_an_exception_when_the_device_description_has_no_rendering_control();
    void throw_an_exception_when_the_rendering_control_description_is_not_correct_data();
    void throw_an_exception_when_the_rendering_control_description_is_not_correct();
    /**
     * @test The media renderer should send the correct message when calling get volume.
     */
    void send_correct_soap_message_when_calling_get_volume();

    /**
     * @test The media renderer should send the correct message when calling set volume.
     */
    void send_correct_soap_message_when_calling_set_volume();

    /**
     * @test The media renderer should handle volume and notifies about these changes.
     */
    void notify_volume_changes_when_receiving_upnp_events();

    /**
     * @test The media renderer tells that it's unreachable when the RenderingControl event publisher is unreachable.
     */
    void tell_that_it_is_unreachable_when_the_rendering_control_event_publisher_is_unreachable();
    void give_the_volume_range_of_the_rendering_control();
    void give_the_default_volume_range_without_a_range();
    void tell_whether_the_volume_can_be_set();
    void send_correct_soap_message_when_calling_get_mute();
    void send_correct_soap_message_when_calling_set_mute();
    void notify_mute_changes_when_receiving_upnp_events();
    void tell_whether_the_mute_can_be_set();
};
} // namespace UPnPAV
