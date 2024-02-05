// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
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
};
} // namespace UPnPAV
