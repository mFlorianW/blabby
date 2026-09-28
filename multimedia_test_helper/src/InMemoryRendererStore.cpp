// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "InMemoryRendererStore.hpp"

namespace Multimedia::TestHelper
{

InMemoryRendererStore::InMemoryRendererStore(QList<Multimedia::RememberedRenderer> renderers)
    : mRenderers{std::move(renderers)}
{
}

InMemoryRendererStore::~InMemoryRendererStore() = default;

QList<Multimedia::RememberedRenderer> InMemoryRendererStore::load() const
{
    return mRenderers;
}

void InMemoryRendererStore::save(QList<Multimedia::RememberedRenderer> const& renderers)
{
    mRenderers = renderers;
    ++mSaveCount;
}

qsizetype InMemoryRendererStore::saveCount() const noexcept
{
    return mSaveCount;
}

} // namespace Multimedia::TestHelper
