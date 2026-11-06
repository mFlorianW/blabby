// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <QByteArray>
#include <QHash>
#include <utility>

namespace Http
{

/**
 * The name of an HTTP header.
 * HTTP header names are case-insensitive (RFC 9110), so two names compare equal and hash equally regardless of
 * their case. The name keeps the case it was created with, e.g. for serializing a response.
 */
class HeaderName
{
public:
    /**
     * Creates a header name.
     * Implicit, so a header can be looked up with a plain string, e.g. headers().value("SID").
     * @param name The header name.
     */
    HeaderName(QByteArray name) noexcept
        : mName{std::move(name)}
    {
    }

    /**
     * @copydoc HeaderName(QByteArray)
     */
    HeaderName(char const* name)
        : mName{name}
    {
    }

    /**
     * Gives the header name in the case it was created with.
     * @return The header name.
     */
    QByteArray const& name() const noexcept
    {
        return mName;
    }

    friend bool operator==(HeaderName const& lhs, HeaderName const& rhs) noexcept
    {
        return lhs.mName.compare(rhs.mName, Qt::CaseInsensitive) == 0;
    }

    friend bool operator!=(HeaderName const& lhs, HeaderName const& rhs) noexcept
    {
        return not(lhs == rhs);
    }

    friend size_t qHash(HeaderName const& headerName, size_t seed = 0) noexcept
    {
        return qHash(headerName.mName.toLower(), seed);
    }

private:
    QByteArray mName;
};

/**
 * Type alias for HTTP hash table
 */
using Headers = QHash<HeaderName, QByteArray>;

} // namespace Http
