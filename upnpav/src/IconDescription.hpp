// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef ICONDESCRIPTION_H
#define ICONDESCRIPTION_H

#include "blabbyupnpav_export.h"
#include <QString>

namespace UPnPAV
{

class BLABBYUPNPAV_EXPORT IconDescription final
{
public:
    IconDescription();

    IconDescription(QString mimeType, quint32 width, quint32 height, quint32 depth, QString url);

    QString const &mimeType() const noexcept;

    QString const &url() const noexcept;

    quint32 width() const noexcept;

    quint32 height() const noexcept;

    quint32 depth() const noexcept;

    friend bool operator==(IconDescription const &lhs, IconDescription const &rhs) noexcept;
    friend bool operator!=(IconDescription const &lhs, IconDescription const &rhs) noexcept;

private:
    QString m_mimeType;
    QString m_url;
    QString m_baseUrl;

    quint32 m_width{0};
    quint32 m_height{0};
    quint32 m_depth{0};
};

} // namespace UPnPAV

#endif // ICONDESCRIPTION_H
