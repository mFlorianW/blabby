// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once
#include "Provider.hpp"
#include "blabbymultimedia_export.h"
#include <QFileInfo>
#include <QVector>
#include <memory>

namespace Multimedia
{

/**
 * Definition for a list of Provider
 */
using Providers = QVector<std::shared_ptr<Provider>>;

/**
 * A provider looks for shared libraries in the past folders and tries to
 * load them with if they implements the @ref Multimedia::Provider interface.
 */
class BLABBYMULTIMEDIA_EXPORT ProviderLoader final
{
public:
    /**
     * Creates in an instance of the ProviderLoader
     */
    ProviderLoader();

    /**
     * Default Destructor
     */
    ~ProviderLoader();

    /**
     * Disable copy move
     */
    Q_DISABLE_COPY_MOVE(ProviderLoader)
    /**
     * Loads all plugins in the given paths.
     * It will only load plugins that implements the @ref Multimedia::Provider interface.
     * @param paths The folder list for the recursive search for valid provider.
     * @return The list of loaded providers.
     */
    [[nodiscard]] Providers const &load(QStringList const &paths) noexcept;

private:
    void loadProvider(QFileInfo const &provider) noexcept;
    Providers mProviders;
};

} // namespace Multimedia
