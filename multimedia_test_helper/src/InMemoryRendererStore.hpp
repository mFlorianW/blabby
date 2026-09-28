// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "RendererStore.hpp"

namespace Multimedia::TestHelper
{

/**
 * A @ref Multimedia::RendererStore that keeps the remembered @ref Multimedia::Renderer in memory.
 */
class InMemoryRendererStore : public Multimedia::RendererStore
{
public:
    /**
     * Creates an instance of the InMemoryRendererStore.
     * @param renderers The Renderers that are remembered from the start.
     */
    explicit InMemoryRendererStore(QList<Multimedia::RememberedRenderer> renderers = {});

    /**
     * Default destructor
     */
    ~InMemoryRendererStore() override;

    /**
     * Disabled copy and move semantic
     */
    Q_DISABLE_COPY_MOVE(InMemoryRendererStore)

    /**
     * @copydoc Multimedia::RendererStore::load
     */
    QList<Multimedia::RememberedRenderer> load() const override;

    /**
     * @copydoc Multimedia::RendererStore::save
     */
    void save(QList<Multimedia::RememberedRenderer> const& renderers) override;

    /**
     * Gives how often the remembered Renderers were saved.
     * @return The number of @ref InMemoryRendererStore::save calls.
     */
    qsizetype saveCount() const noexcept;

private:
    QList<Multimedia::RememberedRenderer> mRenderers;
    qsizetype mSaveCount = 0;
};

} // namespace Multimedia::TestHelper
