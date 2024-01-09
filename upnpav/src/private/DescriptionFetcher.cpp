// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "DescriptionFetcher.hpp"
#include "DescriptionFetcherBackend.hpp"

namespace UPnPAV
{

DescriptionFetcher::DescriptionFetcher(DescriptionFetcherBackend* backend)
    : QObject()
    , m_backend(backend)
{
    (void)connect(m_backend,
                  &DescriptionFetcherBackend::descriptionFetched,
                  this,
                  &DescriptionFetcher::descriptionFetched);
}

DescriptionFetcher::~DescriptionFetcher() = default;

void DescriptionFetcher::fetchDescription(QUrl const& url)
{
    m_backend->fetchDescriptionFrom(url);
}

} // namespace UPnPAV
