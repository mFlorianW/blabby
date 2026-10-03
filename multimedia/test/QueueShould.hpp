// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "ClockDouble.hpp"
#include "MediaRendererDouble.hpp"
#include "Queue.hpp"
#include "Renderer.hpp"
#include <QObject>

namespace Multimedia
{

class QueueShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~QueueShould() override;
    Q_DISABLE_COPY_MOVE(QueueShould)

private:
    void report(QString const& uri, QString const& duration, QString const& position);
    void finishEntry(QString const& uri);
    void reportPlayedFor(QString const& uri, std::chrono::seconds elapsed);
    void makeIdle(qsizetype currentIndex);
    std::shared_ptr<Renderer> createBathroom();
    void handOverToBathroom(bool seekSupported, QString const& duration, QString const& position);

    UPnPAV::Doubles::MediaRendererDouble* mDevice = nullptr;
    UPnPAV::Doubles::MediaRendererDouble* mBathroomDevice = nullptr;
    UPnPAV::ClockDouble* mClock = nullptr;
    std::shared_ptr<Renderer> mRenderer;
    std::unique_ptr<Queue> mQueue;

private Q_SLOTS:
    void init();
    void cleanup();
    void be_empty_and_idle_at_start();
    void hold_the_playables_and_the_current_entry_after_a_replace();
    void play_the_start_entry_on_the_active_renderer_and_run_after_a_replace();
    void notify_about_a_replace();
    void only_hold_the_playables_without_an_active_renderer();
    void record_the_position_of_the_renderer_while_it_plays_the_current_entry();
    void not_record_the_position_of_another_current_track();
    void not_take_the_position_of_the_previous_track_for_the_next_entry();
    void play_the_next_entry_when_the_current_entry_finished();
    void become_idle_after_the_last_entry_finished();
    void not_advance_on_a_stop_data();
    void not_advance_on_a_stop();
    void follow_the_active_renderer_it_is_given();
    void play_the_entry_at_an_index_from_its_start_and_run();
    void play_the_current_entry_again_from_its_start();
    void only_make_the_entry_at_an_index_current_without_an_active_renderer();
    void ignore_playing_at_an_invalid_index();
    void tell_whether_the_renderer_plays_the_current_entry();
    void notify_when_the_renderer_starts_or_stops_playing_the_current_entry();
    void append_the_playables_at_the_end_and_keep_the_current_entry();
    void notify_about_an_append();
    void make_the_first_added_playable_current_on_an_empty_queue_data();
    void make_the_first_added_playable_current_on_an_empty_queue();
    void insert_the_playables_after_the_current_entry_on_play_next();
    void notify_about_play_next();
    void append_on_play_next_without_a_current_entry();
    void add_a_playable_of_a_source_without_collecting_it_data();
    void add_a_playable_of_a_source_without_collecting_it();
    void collect_the_playables_of_a_container_depth_first_in_source_order_data();
    void collect_the_playables_of_a_container_depth_first_in_source_order();
    void collect_a_container_over_several_pages();
    void collect_a_container_that_contains_itself_only_once();
    void change_only_when_the_collection_completes();
    void insert_a_collected_container_after_the_current_entry_at_completion();
    void leave_the_queue_unchanged_and_report_a_failed_collection();
    void cancel_a_collection();
    void cancel_a_collection_by_a_new_one();
    void abort_a_collection_when_its_source_disappears();
    void play_the_following_entry_and_run_on_next();
    void tell_whether_previous_and_next_are_available();
    void ignore_next_on_the_last_entry();
    void play_the_preceding_entry_on_previous_within_the_first_3_seconds();
    void restart_the_current_entry_on_previous_after_3_seconds();
    void play_the_preceding_entry_on_a_second_previous_right_after_a_restart();
    void restart_the_current_entry_on_previous_by_loading_it_again_without_seek_support();
    void restart_the_first_entry_on_previous();
    void start_the_new_current_entry_of_an_idle_queue_data();
    void start_the_new_current_entry_of_an_idle_queue();
    void take_the_renderer_back_from_another_controller_data();
    void take_the_renderer_back_from_another_controller();
    void restart_the_current_entry_on_previous_by_loading_it_again_after_another_controller_took_over();
    void remove_an_entry_that_is_not_current_without_affecting_playback_data();
    void remove_an_entry_that_is_not_current_without_affecting_playback();
    void notify_about_a_remove();
    void play_the_next_entry_when_the_current_entry_of_a_running_queue_is_removed();
    void make_the_next_entry_current_when_the_current_entry_of_an_idle_queue_is_removed();
    void stop_and_become_idle_when_the_last_remaining_current_entry_is_removed_data();
    void stop_and_become_idle_when_the_last_remaining_current_entry_is_removed();
    void not_stop_another_controller_when_the_last_remaining_current_entry_is_removed();
    void ignore_removing_at_an_invalid_index();
    void move_entries_without_interrupting_playback_data();
    void move_entries_without_interrupting_playback();
    void notify_about_a_move();
    void ignore_an_invalid_move();
    void empty_the_queue_make_it_idle_and_stop_the_renderer_on_clear();
    void not_stop_another_controller_on_clear();
    void restore_a_snapshot_without_interrupting_the_current_entry();
    void play_the_restored_current_entry_again_when_it_played_before_data();
    void play_the_restored_current_entry_again_when_it_played_before();
    void restore_an_idle_queue_without_playing();
    void hand_over_a_running_queue_to_the_new_active_renderer_data();
    void hand_over_a_running_queue_to_the_new_active_renderer();
    void hand_over_a_running_queue_after_another_controller_took_over();
    void not_hand_over_an_idle_queue();
    void play_a_stream_from_its_start_on_a_hand_over();
    void stop_the_previous_renderer_while_it_loads_the_current_entry_on_a_hand_over();
    void not_stop_a_renderer_the_queue_was_still_handed_over_to();
    void become_idle_when_the_new_renderer_fails_to_initialize_on_a_hand_over();
    void not_hand_over_when_paused_during_a_pending_hand_over();
    void become_idle_and_keep_the_current_entry_when_the_active_renderer_goes_offline();
    void become_idle_when_the_active_renderer_is_unset();
    void pause_the_renderer_and_become_idle_on_toggle_while_it_plays_data();
    void pause_the_renderer_and_become_idle_on_toggle_while_it_plays();
    void resume_the_current_entry_and_run_on_toggle();
    void continue_the_current_entry_at_the_last_known_position_on_toggle_of_an_idle_queue();
    void resume_the_renderer_on_toggle_of_an_empty_queue();
    void ignore_toggling_while_a_playback_control_is_pending_or_transitioning();
};

} // namespace Multimedia
