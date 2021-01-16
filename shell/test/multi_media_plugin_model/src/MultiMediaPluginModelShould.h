/**
 ** This file is part of the Blabby project.
 ** Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as
 ** published by the Free Software Foundation, either version 2 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
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
    void give_qml_url_for_valid_index();
    void give_status_if_a_plugin_is_active();
    void give_a_icon_url_for_a_plugin();
    void should_give_a_multimedia_plugin_object();
};

} // namespace Shell

#endif // MULTIMEDIAPLUGINMODELSHOULD_H
