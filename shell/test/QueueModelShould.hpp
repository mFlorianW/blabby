// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "MediaRendererDoubleFactory.hpp"
#include "MediaRendererModel.hpp"
#include "QueueModel.hpp"
#include "ServiceProviderDouble.hpp"
#include <QObject>
#include <memory>

namespace Shell
{
class QueueModelShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~QueueModelShould() override;
    Q_DISABLE_COPY_MOVE(QueueModelShould)

private:
    void activate(QString const& name);
    UPnPAV::Doubles::MediaRendererDouble* device(QString const& name) const noexcept;

    UPnPAV::Doubles::ServiceProviderDouble* mServiceProvider = nullptr;
    UPnPAV::Doubles::MediaRendererDoubleFactory* mRendererFactory = nullptr;
    std::unique_ptr<MediaRendererModel> mRendererModel = nullptr;
    std::unique_ptr<QueueModel> mModel = nullptr;

private Q_SLOTS:
    void init();
    void cleanup();
    void be_empty_and_not_running_at_start();
    void give_the_role_names();
    void give_the_title_artist_and_artwork_of_the_entries_and_mark_the_current_entry();
    void reset_when_the_queue_is_replaced();
    void play_on_the_active_renderer_and_run_after_a_replace();
    void follow_a_switch_of_the_active_renderer();
    void only_hold_the_entries_without_an_active_renderer();
    void mark_the_next_entry_as_current_when_the_queue_advances();
    void give_the_album_and_the_duration_of_the_entries();
    void fall_back_to_the_file_name_as_title();
    void summarize_the_entry_count_and_the_total_duration_data();
    void summarize_the_entry_count_and_the_total_duration();
    void notify_about_a_changed_summary();
    void play_the_entry_at_a_row();
    void only_make_the_entry_at_a_row_current_without_an_active_renderer();
    void tell_whether_the_active_renderer_plays_the_current_entry();
    void append_a_playable_and_play_it();
    void only_append_a_playable_and_make_it_current_without_an_active_renderer();
    void tell_whether_previous_and_next_are_available();
    void notify_when_the_availability_of_previous_and_next_changed();
    void step_to_the_next_and_the_previous_entry();
    void remove_the_row_of_an_entry();
    void mark_the_next_entry_as_current_when_the_current_entry_is_removed();
    void move_the_row_of_an_entry_data();
    void move_the_row_of_an_entry();
    void clear_all_rows();
    void undo_a_remove_or_a_clear_data();
    void undo_a_remove_or_a_clear();
    void ignore_undo_without_a_remove_or_a_clear();
    void insert_the_rows_of_playables_played_next_or_added_to_the_queue();
    void make_the_first_added_playable_current_in_an_empty_queue();
    void tell_while_a_container_is_collected();
    void cancel_the_collection_of_a_container();
    void report_a_failed_collection_with_the_title_of_the_container();
    void not_undo_after_playables_were_added();
    void pause_the_active_renderer_and_stop_running_on_toggle();
    void stop_the_active_renderer_that_cannot_pause_on_toggle();
    void resume_a_paused_or_stopped_active_renderer_on_toggle_data();
    void resume_a_paused_or_stopped_active_renderer_on_toggle();
    void continue_the_current_entry_of_an_idle_queue_on_toggle();
    void ignore_toggling_while_a_call_is_pending_or_transitioning();
    void hand_over_a_running_queue_to_the_new_active_renderer();
    void stop_running_when_the_active_renderer_goes_offline();
};

} // namespace Shell
