// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <QObject>

namespace Multimedia
{

class RendererShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~RendererShould() override;
    Q_DISABLE_COPY_MOVE(RendererShould)
private Q_SLOTS:
    void request_supported_protocols_on_init();
    void signal_that_initialization_successful_finished();
    void signal_that_initialization_unsuccessful_finished();
    void call_avtransport_uri_on_playback_request();
    void call_play_on_successful_avtransporturi_request();
    void not_call_avtransporturi_with_unsupported_items();
    void signal_playback_failed_on_avtransporturi_call_failed();
    void signal_playback_failed_on_playcall_failed();
    void map_upnp_devices_to_renderer_device_states_data();
    void map_upnp_devices_to_renderer_device_states();
};

} // namespace Multimedia
