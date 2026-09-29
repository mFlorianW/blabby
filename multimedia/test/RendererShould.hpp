// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "ClockDouble.hpp"
#include "MediaRendererDouble.hpp"
#include "Renderer.hpp"
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
    void give_the_identity_manufacturer_model_and_address_of_the_renderer();
    void be_online_when_created_for_a_device();
    void be_offline_with_the_remembered_details_when_created_from_a_remembered_renderer();
    void go_online_with_the_refreshed_details_of_the_device();
    void go_offline_and_keep_the_last_known_details();
    void give_no_playback_state_while_offline();
    void ignore_playback_requests_while_offline();
    void initialize_again_when_an_initialized_renderer_goes_online_again();
    void not_initialize_an_uninitialized_renderer_when_it_goes_online();
    void give_no_volume_while_offline();
    void go_offline_when_the_device_does_not_answer_a_call_data();
    void go_offline_when_the_device_does_not_answer_a_call();
    void stay_online_when_the_device_answers_a_call_with_an_error();
    void go_offline_when_the_event_publisher_of_the_device_is_unreachable();
    void stay_online_when_a_dropped_device_was_unreachable();
    void give_the_current_track_reported_by_the_device_events();
    void fall_back_for_missing_current_track_details_data();
    void fall_back_for_missing_current_track_details();
    void give_the_current_track_of_the_polled_position_info();
    void not_notify_about_an_unchanged_current_track();
    void poll_the_position_info_every_second_while_tracked_and_playing();
    void skip_a_poll_while_the_previous_request_is_pending();
    void not_poll_while_not_tracked();
    void not_poll_while_not_playing_data();
    void not_poll_while_not_playing();
    void refresh_the_position_info_after_a_playback_state_change();
    void stop_polling_when_the_tracking_is_switched_off();
    void refresh_the_position_info_when_a_tracked_renderer_goes_online();
    void give_no_current_track_while_offline();
    void give_the_album_and_the_year_of_the_current_track_data();
    void give_the_album_and_the_year_of_the_current_track();
    void give_the_format_of_the_current_track_data();
    void give_the_format_of_the_current_track();
    void tell_whether_it_can_pause();
    void keep_the_playback_state_while_transitioning();
    void report_a_failed_playback_control_call_data();
    void report_a_failed_playback_control_call();
    void tell_while_a_playback_control_call_is_pending();
    void give_the_position_and_the_duration_of_the_polled_position_info();
    void give_no_duration_for_a_stream_data();
    void give_no_duration_for_a_stream();
    void give_no_position_and_duration_while_offline();
    void seek_by_relative_time();
    void refresh_the_position_info_after_a_seek();
    void ignore_a_position_requested_before_a_seek_finished();
    void ignore_positions_polled_while_a_seek_is_in_flight();
    void report_a_failed_seek();
    void tell_whether_it_can_seek();
    void coalesce_volume_requests();
    void not_send_a_requested_volume_that_was_already_sent();
    void report_a_failed_volume_change();
    void give_the_volume_range_and_whether_the_volume_can_be_controlled();

private:
    std::unique_ptr<Renderer> createTrackedRenderer(UPnPAV::MediaDevice::State state);

    UPnPAV::ClockDouble* mClock = nullptr;
    std::unique_ptr<UPnPAV::Doubles::MediaRendererDouble> mUpnpRenderer = nullptr;
    UPnPAV::Doubles::MediaRendererDouble* mUpnpRendererRaw = nullptr;
};

} // namespace Multimedia
