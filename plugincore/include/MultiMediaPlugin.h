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
#ifndef MULTIMEDIAPLUGIN_H
#define MULTIMEDIAPLUGIN_H

#include "PluginCore_Export.h"
#include "PluginVersion.h"
#include <QObject>
#include <QUuid>

namespace PluginCore
{

/**
 * Generic defintion of plugin, which can be loaded by
 * Blabby shell
 */
class PLUGINCORE_EXPORT MultiMediaPlugin
{
public:
    virtual ~MultiMediaPlugin()
    {
    }

    /**
     * Give a human readable name of the plugin.
     *
     * @return The name of the plugin.
     */
    virtual QString getPluginName() const = 0;

    /**
     * Gives the actual version of the plugin
     *
     * @return The plugin version
     */
    virtual PluginVersion getPluginVersion() const = 0;

    /**
     * Gives the global unique identifier for the plugin.
     *
     * @return The global unique identifier for the plguin.
     */
    virtual QUuid getPluginIdentifier() const = 0;

    /**
     * Load is called when ever a plugin is loaded by the shell. Load shall
     * be used to initialize all long living variables in the plugin.
     *
     * @return True when the plugin is initialized, otherwise false.
     */
    virtual bool load() = 0;

    /**
     * Unload is called when a ever the plugin is unloaded by the shell.
     * E.g the user changes the plugin.
     *
     * @return True when the stop of the plugin is performed succesful,
     *         otherwise false
     */
    virtual bool unload() = 0;

    /**
     * Gives the url to the main qml that shall be loaded.
     *
     * @return The url to main qml.
     */
    virtual QUrl getMainQML() const = 0;
};

} // namespace PluginCore

Q_DECLARE_INTERFACE(PluginCore::MultiMediaPlugin, "de.blabby.MultiMediaPlugin")


#endif // FREESMARTTVPLUGIN_H
