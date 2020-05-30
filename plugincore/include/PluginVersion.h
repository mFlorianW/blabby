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
#ifndef PLUGINVERSION_H
#define PLUGINVERSION_H

#include <QtGlobal>
#include <PluginCore_Export.h>

namespace PluginCore
{

/**
 * Struct which handles the version of the plugin
 */
struct PLUGINCORE_EXPORT PluginVersion{
    quint32 major = 0;
    quint32 minor = 0;
    quint32 patch = 0;
};

} /* end namespace */

#endif // PLUGINVERSION_H
