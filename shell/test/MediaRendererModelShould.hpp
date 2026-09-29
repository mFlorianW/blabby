// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "InMemoryRendererStore.hpp"
#include "MediaRendererDoubleFactory.hpp"
#include "MediaRendererModel.hpp"
#include "ServiceProviderDouble.hpp"
#include <QObject>
#include <memory>

namespace Shell
{
class MediaRendererModelShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~MediaRendererModelShould() override;
    Q_DISABLE_COPY_MOVE(MediaRendererModelShould)

private:
    void restart();
    QStringList rendererNames() const;
    QString name(int row) const;
    bool isActive(int row) const;
    Multimedia::Renderer::Availability availability(int row) const;

    std::shared_ptr<Multimedia::TestHelper::InMemoryRendererStore> mStore;
    UPnPAV::Doubles::ServiceProviderDouble* mServiceProvider = nullptr;
    UPnPAV::Doubles::MediaRendererDoubleFactory* mRendererFactory = nullptr;
    std::unique_ptr<MediaRendererModel> mModel = nullptr;

private Q_SLOTS:
    void init();
    void give_correct_display_roles_for_the_ui();
    void start_a_mediarenderer_discover_on_init();
    void increase_the_rowCount_on_new_connected_mediarenderer();
    void keep_a_disconnected_renderer_listed_as_offline();
    void give_the_name_and_playback_state_of_the_renderer();
    void notify_about_a_changed_playback_state();
    void give_the_manufacturer_model_and_address_of_the_renderer();
    void order_the_renderers_alphabetically_by_name();
    void keep_the_renderers_ordered_when_renderers_appear_and_disappear();
    void have_no_active_renderer_at_start();
    void activate_the_renderer_of_the_passed_index();
    void switch_the_active_renderer_without_stopping_the_previous_one();
    void ignore_activating_the_active_renderer_again();
    void ignore_activating_an_invalid_index();
    void clear_the_active_renderer_when_it_disconnects();
    void clear_the_active_renderer_when_its_device_does_not_answer();
    void keep_the_active_renderer_when_another_renderer_disconnects();
    void be_scanning_after_start_until_the_discovery_is_finished();
    void start_a_new_discovery_on_rescan();
    void ignore_a_rescan_while_scanning();
    void give_the_availability_of_the_renderer();
    void list_the_remembered_renderers_as_offline_at_start();
    void bring_a_remembered_renderer_online_with_refreshed_details_when_it_is_discovered_again();
    void remember_the_discovered_renderers_across_a_restart();
    void keep_the_last_known_address_of_an_offline_renderer_across_a_restart();
    void order_online_renderers_before_offline_ones_and_alphabetically_within_each();
    void move_a_renderer_behind_the_online_ones_when_it_goes_offline();
    void ignore_activating_an_offline_renderer();
};

} // namespace Shell
