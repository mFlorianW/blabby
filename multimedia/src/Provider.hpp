// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "Source.hpp"
#include "blabbymultimedia_export.h"
#include <QObject>
#include <QVariant>

namespace Multimedia
{
/**
 * Alias for a list of media sources
 */
using MediaSources = QVector<std::shared_ptr<Source>>;

/**
 * Provider plugin version
 */
struct BLABBYMULTIMEDIA_EXPORT ProviderVersion
{
    quint32 major = 0;
    quint32 minor = 0;
    quint32 patch = 0;

    /**
     * Converts the version in the string representation of <major>.<minor>.<patch>.
     * @return The version as string.
     */
    QString toString() const noexcept;

    BLABBYMULTIMEDIA_EXPORT friend auto operator<=>(ProviderVersion const&, ProviderVersion const&) = default;
};

/**
 * A setting a Provider describes, the shell shows it on the Settings screen with its own controls, keeps its value and
 * passes changes to the Provider.
 */
struct BLABBYMULTIMEDIA_EXPORT ProviderSetting
{
    /**
     * The kind of a setting, it decides which control shows it.
     */
    enum class Kind
    {
        Switch, ///< A boolean value, shown as a switch.
        Action, ///< A button without a value, a tap is passed to the Provider.
    };

    /**
     * The stable key of the setting, the shell stores the value under it.
     */
    QString key;

    /**
     * The translated title of the setting.
     */
    QString title;

    /**
     * A translated short description shown below the title, empty when the setting has none.
     */
    QString subtitle;

    /**
     * The kind of the setting.
     */
    Kind kind = Kind::Switch;

    /**
     * The value of a switch while it isn't stored, unused for an action.
     */
    bool defaultValue = false;

    BLABBYMULTIMEDIA_EXPORT friend bool operator==(ProviderSetting const&, ProviderSetting const&) = default;
};

/**
 * Base class for every Provider plugin.
 * A derived class of this has to implement the @ref Provider::name() "init" function.
 * The @ref Proivder::name() "init" will be autommatically called by the ProviderLoader on application start.
 */
class BLABBYMULTIMEDIA_EXPORT Provider : public QObject
{
    Q_OBJECT
public:
    /**
     * Default destructor
     */
    ~Provider() override;

    /**
     * Deleted move and copy operations
     */
    Q_DISABLE_COPY_MOVE(Provider)

    /**
     * Gives the name of the Provider.
     * @return A human readable name of the provider.
     */
    QString const& name() const noexcept;

    /**
     * Gives the version of the plugin.
     * @return Gives the version of the plugin.
     */
    ProviderVersion const& version() const noexcept;

    /**
     * This function must be implemented by every derived class.
     * Here should the initialization start of the plugin.
     *
     * @note
     * The default implementation just returns true.
     *
     * @return true everything initialized and false on a error.
     */
    virtual bool init() noexcept;

    /**
     * Gives list with all media sources of the plugin.
     *
     * @note
     * The default implementation returns an empty list.
     *
     * @return A list with all media sources.
     */
    virtual MediaSources sources() const noexcept;

    /**
     * Gives the translated title the shell shows for the Provider, e.g. as the title of its Settings section.
     *
     * @note
     * The default implementation returns an empty string, the shell shows @ref Provider::name() then.
     *
     * @return The translated title or an empty string.
     */
    virtual QString displayTitle() const noexcept;

    /**
     * Gives the settings of the Provider in the order the shell shows them.
     * The Provider never reads or writes their values itself, the shell keeps them and passes them with
     * @ref Provider::setSettingValue() and the taps on an action with @ref Provider::triggerAction().
     *
     * @note
     * The default implementation returns an empty list, the Provider has no settings.
     *
     * @return The settings of the Provider.
     */
    virtual QList<ProviderSetting> settings() const noexcept;

    /**
     * Applies the value of a setting. The shell calls it at start for each switch with its stored value, or its default
     * when none is stored, and whenever the user changes it.
     *
     * @note
     * The default implementation does nothing.
     *
     * @param key The key of the setting.
     * @param value The value of the setting.
     */
    virtual void setSettingValue(QString const& key, QVariant const& value) noexcept;

    /**
     * Performs the action setting the user tapped.
     *
     * @note
     * The default implementation does nothing.
     *
     * @param key The key of the action setting.
     */
    virtual void triggerAction(QString const& key) noexcept;

Q_SIGNALS:
    /**
     * This signal shall be emitted when ever a source is added to provider.
     * As an example a new MediaServer appears on the network, a USB-Stick got mounted etc.
     */
    void sourceAdded(std::shared_ptr<Multimedia::Source> const& source);

    /**
     * This signal shall be emitted when ever a source is removed in the provider.
     * As an exmaple a MediaServer disappears on the network or USB-Stick got removed etc.
     */
    void sourceRemoved(std::shared_ptr<Multimedia::Source> const& source);

protected:
    Provider(QString name, ProviderVersion version);

private:
    QString mName{""};
    ProviderVersion mVersion{.major = quint32{0}, .minor = quint32{0}, .patch = quint32{0}};
};

} // namespace Multimedia

Q_DECLARE_INTERFACE(Multimedia::Provider, "org.blabby.Provider");
