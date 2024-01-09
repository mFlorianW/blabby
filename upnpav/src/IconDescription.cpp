// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "IconDescription.hpp"

#include <QUrl>
#include <utility>

namespace UPnPAV
{

IconDescription::IconDescription() = default;

IconDescription::IconDescription(QString mimeType, quint32 width, quint32 height, quint32 depth, QString url)
    : m_mimeType(std::move(mimeType))
    , m_url(std::move(url))
    , m_width(width)
    , m_height(height)
    , m_depth(depth)
{
}

// void IconDescription::setBaseUrl(const QString &baseUrl)
//{
//     m_baseUrl = baseUrl;

//    if(QUrl{m_url}.isRelative())
//    {
//        m_url = m_url.startsWith("/") ? m_baseUrl + m_url
//                                      : m_baseUrl + "/" + m_url;
//    }
//}

QString const& IconDescription::mimeType() const noexcept
{
    return m_mimeType;
}

QString const& IconDescription::url() const noexcept
{
    return m_url;
}

quint32 IconDescription::width() const noexcept
{
    return m_width;
}

quint32 IconDescription::height() const noexcept
{
    return m_height;
}

quint32 IconDescription::depth() const noexcept
{
    return m_depth;
}

bool operator==(IconDescription const& lhs, IconDescription const& rhs) noexcept
{
    if (&lhs == &rhs) {
        return true;
    }

    return ((lhs.mimeType() == rhs.mimeType()) && (lhs.m_url == rhs.m_url) && (lhs.m_width == rhs.m_width) &&
            (lhs.m_height == rhs.m_height) && (lhs.m_depth == rhs.m_depth));
}

bool operator!=(IconDescription const& lhs, IconDescription const& rhs) noexcept
{
    return !(lhs == rhs);
}

} // namespace UPnPAV
