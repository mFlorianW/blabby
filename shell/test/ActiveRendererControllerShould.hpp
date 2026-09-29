// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "ActiveRendererController.hpp"
#include "MediaRendererDoubleFactory.hpp"
#include "MediaRendererModel.hpp"
#include "ServiceProviderDouble.hpp"
#include <QObject>
#include <memory>

namespace Shell
{
class ActiveRendererControllerShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~ActiveRendererControllerShould() override;
    Q_DISABLE_COPY_MOVE(ActiveRendererControllerShould)

private:
    void activate(QString const& name);
    UPnPAV::Doubles::MediaRendererDouble* kitchen() const noexcept;

    UPnPAV::Doubles::ServiceProviderDouble* mServiceProvider = nullptr;
    UPnPAV::Doubles::MediaRendererDoubleFactory* mRendererFactory = nullptr;
    std::unique_ptr<MediaRendererModel> mModel = nullptr;
    std::unique_ptr<ActiveRendererController> mController = nullptr;

private Q_SLOTS:
    void init();
    void have_no_active_renderer_at_start();
    void follow_the_active_renderer();
    void give_the_playback_state_of_the_active_renderer();
    void notify_about_a_changed_playback_state_of_the_active_renderer();
    void follow_a_switch_to_another_active_renderer();
    void ignore_playback_state_changes_of_the_previous_active_renderer();
    void report_the_active_renderer_going_offline();
    void track_the_position_of_the_active_renderer_only();
    void give_the_current_track_of_the_active_renderer();
    void give_no_current_track_without_an_active_renderer();
    void pause_a_playing_active_renderer();
    void stop_a_playing_active_renderer_that_cannot_pause();
    void resume_a_paused_or_stopped_active_renderer_data();
    void resume_a_paused_or_stopped_active_renderer();
    void ignore_toggling_the_playback_while_a_call_is_pending();
    void ignore_toggling_the_playback_while_transitioning();
    void give_whether_the_active_renderer_can_pause_and_is_transitioning();
    void report_a_failed_control_call_with_the_renderer_name();
    void give_the_position_and_the_duration_of_the_active_renderer();
    void give_no_position_and_duration_without_an_active_renderer();
    void seek_in_the_current_track_of_the_active_renderer();
    void give_the_volume_of_the_active_renderer();
    void set_the_volume_of_the_active_renderer();
};

} // namespace Shell
