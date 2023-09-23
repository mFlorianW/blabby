// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MULTIMEDIAPLUGINMODELSHOULD_H
#define MULTIMEDIAPLUGINMODELSHOULD_H

#include <QObject>

namespace Shell
{

class MultiMediaPluginModelShould : public QObject
{
    Q_OBJECT
public:
    MultiMediaPluginModelShould();

private Q_SLOTS:
    void give_the_correct_role_names();
    void give_the_number_of_loaded_multimedia_plugins();
    void give_the_plugin_the_name();
    void give_empty_variant_for_invalid_index();
    void give_status_if_a_plugin_is_active();
    void give_a_icon_url_for_a_plugin();
    void should_give_a_multimedia_plugin_object();
};

} // namespace Shell

#endif // MULTIMEDIAPLUGINMODELSHOULD_H
