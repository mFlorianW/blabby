// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <QObject>

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
    void give_the_role_names_for_the_ui();
    void list_the_about_section();
    void pick_the_first_section_from_the_start();
    void show_the_blabby_version_and_the_qt_version_in_about();
    void show_each_loaded_provider_with_its_version_in_about();
    void ignore_picking_an_invalid_section();
    void ignore_picking_the_picked_section_again();
};

} // namespace Shell
