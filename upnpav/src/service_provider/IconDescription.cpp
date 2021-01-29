// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "IconDescription.h"

#include <QUrl>

namespace UPnPAV
{

IconDescription::IconDescription()
{
}

IconDescription::IconDescription(const QString &mimeType,
                                 quint32 width,
                                 quint32 height,
                                 quint32 depth,
                                 const QString &url)
    : m_mimeType(mimeType)
    , m_url(url)
    , m_width(width)
    , m_height(height)
    , m_depth(depth)
{
}

//void IconDescription::setBaseUrl(const QString &baseUrl)
//{
//    m_baseUrl = baseUrl;

//    if(QUrl{m_url}.isRelative())
//    {
//        m_url = m_url.startsWith("/") ? m_baseUrl + m_url
//                                      : m_baseUrl + "/" + m_url;
//    }
//}

const QString &IconDescription::mimeType() const noexcept
{
    return m_mimeType;
}

const QString &IconDescription::url() const noexcept
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

bool operator==(const IconDescription &lhs, const IconDescription &rhs) noexcept
{
    if(&lhs == &rhs)
    {
        return true;
    }

    return ((lhs.mimeType() == rhs.mimeType()) &&
            (lhs.m_url == rhs.m_url) &&
            (lhs.m_width == rhs.m_width) &&
            (lhs.m_height == rhs.m_height) &&
            (lhs.m_depth == rhs.m_depth));
}

bool operator!=(const IconDescription &lhs, const IconDescription &rhs) noexcept
{
    return !(lhs == rhs);
}

} //namespace UPnPAV
