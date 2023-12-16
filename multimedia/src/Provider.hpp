// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include "Source.hpp"
#include "blabbymultimedia_export.h"
#include <QObject>

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

    BLABBYMULTIMEDIA_EXPORT friend auto operator<=>(ProviderVersion const &, ProviderVersion const &) = default;
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
    QString const &name() const noexcept;

    /**
     * Gives the version of the plugin.
     * @return Gives the version of the plugin.
     */
    ProviderVersion const &version() const noexcept;

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

Q_SIGNALS:
    /**
     * This signal shall be emitted when ever a source is added to provider.
     * As an example a new MediaServer appears on the network, a USB-Stick got mounted etc.
     */
    void sourceAdded(std::shared_ptr<Source> const &source);

    /**
     * This signal shall be emitted when ever a source is removed in the provider.
     * As an exmaple a MediaServer disappears on the network or USB-Stick got removed etc.
     */
    void sourceRemoved(std::shared_ptr<Source> const &source);

protected:
    Provider(QString name, ProviderVersion version);

private:
    QString mName{""};
    ProviderVersion mVersion{.major = quint32{0}, .minor = quint32{0}, .patch = quint32{0}};
};

} // namespace Multimedia

Q_DECLARE_INTERFACE(Multimedia::Provider, "org.blabby.Provider");
