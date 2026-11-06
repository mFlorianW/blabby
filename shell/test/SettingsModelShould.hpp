// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "InMemorySettingsStore.hpp"
#include <QObject>
#include <memory>

namespace Shell
{

class SettingsModelShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~SettingsModelShould() override;
    Q_DISABLE_COPY_MOVE(SettingsModelShould)

private Q_SLOTS:
    void init();
    void give_the_role_names_for_the_ui();
    void list_the_renderers_section_and_then_about();
    void pick_the_renderers_section_from_the_start();
    void show_the_remember_last_renderer_switch_on_by_default();
    void show_the_stored_value_of_the_remember_last_renderer_switch();
    void store_the_toggled_value_of_the_remember_last_renderer_switch();
    void ignore_toggling_a_row_that_is_no_switch();
    void show_the_blabby_version_and_the_qt_version_in_about();
    void show_each_loaded_provider_with_its_version_in_about();
    void show_the_display_title_of_a_provider_in_about();
    void ignore_picking_an_invalid_section();
    void ignore_picking_the_picked_section_again();
    void pass_each_provider_its_stored_values_or_the_defaults_at_start();
    void list_a_section_for_each_provider_with_settings_ordered_by_title_between_renderers_and_about();
    void show_the_settings_of_a_provider_in_its_section();
    void store_a_toggled_provider_switch_and_pass_it_to_the_provider();
    void pass_a_tapped_action_to_the_provider();
    void ignore_triggering_a_row_that_is_no_action();

private:
    std::shared_ptr<InMemorySettingsStore> mSettings;
};

} // namespace Shell
