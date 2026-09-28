// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "RendererStore.hpp"
#include "blabbymultimedia_export.h"

namespace Multimedia
{

/**
 * A @ref Multimedia::RendererStore that keeps the remembered @ref Multimedia::Renderer in a JSON file.
 */
class BLABBYMULTIMEDIA_EXPORT JsonRendererStore : public RendererStore
{
public:
    /**
     * Creates an instance of the JsonRendererStore.
     * @param filePath The path of the JSON file, missing parent directories are created on save.
     */
    explicit JsonRendererStore(QString filePath);

    /**
     * Default destructor
     */
    ~JsonRendererStore() override;

    /**
     * Disabled copy and move semantic
     */
    Q_DISABLE_COPY_MOVE(JsonRendererStore)

    /**
     * @copydoc Multimedia::RendererStore::load
     * A missing or corrupt file gives no remembered Renderers.
     */
    QList<RememberedRenderer> load() const override;

    /**
     * @copydoc Multimedia::RendererStore::save
     */
    void save(QList<RememberedRenderer> const& renderers) override;

private:
    QString mFilePath;
};

} // namespace Multimedia
