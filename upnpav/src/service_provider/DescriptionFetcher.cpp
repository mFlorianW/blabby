// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "DescriptionFetcher.h"
#include "DescriptionFetcherBackend.h"

namespace UPnPAV
{


DescriptionFetcher::DescriptionFetcher(DescriptionFetcherBackend *backend)
    : QObject()
    , m_backend(backend)
{
    (void)connect(m_backend, &DescriptionFetcherBackend::descriptionFetched, this, &DescriptionFetcher::descriptionFetched);
}

DescriptionFetcher::~DescriptionFetcher() = default;

void DescriptionFetcher::fetchDescription(const QUrl &url)
{
    m_backend->fetchDescriptionFrom(url);
}

} // namespace UPnPAV
