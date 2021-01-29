// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef MULTIMEDIAPLUGIN_H
#define MULTIMEDIAPLUGIN_H

#include "PluginCore_Export.h"
#include "PluginVersion.h"
#include <QObject>
#include <QUuid>
#include <memory>

class QQmlContext;

namespace PluginCore
{
class MultimediaPluginPrivate;

/**
 * Generic defintion of plugin, which can be loaded by
 * Blabby shell
 */
class PLUGINCORE_EXPORT MultimediaPlugin : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MultimediaPlugin)
public:
    ~MultimediaPlugin() override;

    /**
     * Give a human readable name of the plugin.
     *
     * @return The name of the plugin.
     */
    virtual QString pluginName() const = 0;

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
     * @param context The QML context that can be used to set context properties that are needed for the plugin.
     * @return True when the plugin is initialized, otherwise false.
     */
    virtual bool load(QQmlContext *conntext) = 0;

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
    virtual QUrl mainQMLUrl() const = 0;

    /**
     * Gives the url to the icon of the plugin.
     *
     * @return The url to main qml.
     */
    virtual QUrl pluginIconUrl() const = 0;

    /**
     * Activate view, which then is displayed.
     * @param view The URL to the view.
     */
    void setActiveView(const QUrl &view) noexcept;

    /**
     * Gives the active of the multimedia plugin.
     * @return The url to the active view of the plugin or empty when now view set.
     */
    QUrl activeView() const noexcept;

    /**
     * This function is called is by the shell when back button in the UI is pressed.
     * A plugin should return true when the back button is handled in the plugin otherwise
     * false. The default implementation returns false.
     *
     * @return True back button handled by plugin otherwise false.
     */
    virtual bool handleBackButton();

Q_SIGNALS:
    /**
     * This signal is emitted when the active view is changed.
     */
    void activeViewChanged();

protected:
    MultimediaPlugin();

private:
    std::unique_ptr<MultimediaPluginPrivate> d;
};

} // namespace PluginCore

Q_DECLARE_INTERFACE(PluginCore::MultimediaPlugin, "de.blabby.MultiMediaPlugin")

#endif // MULTIMEDIAPLUGIN_H
