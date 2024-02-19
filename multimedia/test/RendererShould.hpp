// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "MediaRendererDouble.hpp"
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
    void init();
    void request_supported_protocols_on_init();
    void signal_that_initialization_successful_finished();
    void signal_that_initialization_unsuccessful_finished();
    void call_avtransport_uri_on_playback_request();
    void call_play_on_successful_avtransporturi_request();
    void not_call_avtransporturi_with_unsupported_items();
    void signal_playback_failed_on_avtransporturi_call_failed();
    void signal_playback_failed_on_playcall_failed();
    void map_upnp_devices_states_to_renderer_device_states_data();
    void map_upnp_devices_states_to_renderer_device_states();
    void stop_request_the_playback();
    void send_pause_request();
    void resume_the_playback_when_the_states_are_stop_and_pause();
    void request_master_volume_on_init_for_instance_id_0();
    void give_master_volume_and_notify_about_changes();
    void set_volume_of_upnpav_media_renderer();

private:
    std::unique_ptr<UPnPAV::Doubles::MediaRendererDouble> mUpnpRenderer = nullptr;
    UPnPAV::Doubles::MediaRendererDouble* mUpnpRendererRaw = nullptr;
};

} // namespace Multimedia
