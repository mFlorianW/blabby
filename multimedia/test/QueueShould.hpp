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

    UPnPAV::Doubles::MediaRendererDouble* mDevice = nullptr;
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
    void append_to_an_empty_queue_without_a_current_entry();
};

} // namespace Multimedia
