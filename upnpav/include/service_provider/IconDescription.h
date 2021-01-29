// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef ICONDESCRIPTION_H
#define ICONDESCRIPTION_H

#include "UPnP_Export.h"

#include <QString>

namespace UPnPAV
{

class UPNP_EXPORT IconDescription final
{
public:
    IconDescription();

    IconDescription(const QString &mimeType,
                    quint32 width,
                    quint32 height,
                    quint32 depth,
                    const QString &url);

    const QString &mimeType() const noexcept;

    const QString &url() const noexcept;

    quint32 width() const noexcept;

    quint32 height() const noexcept;

    quint32 depth() const noexcept;

    friend bool operator==(const IconDescription &lhs, const IconDescription &rhs) noexcept;
    friend bool operator!=(const IconDescription &lhs, const IconDescription &rhs) noexcept;

private:
    QString m_mimeType;
    QString m_url;
    QString m_baseUrl;

    quint32 m_width;
    quint32 m_height;
    quint32 m_depth;
};

} // namespace UPnPAV

#endif // ICONDESCRIPTION_H
