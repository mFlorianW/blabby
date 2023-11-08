// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include "blabbymediaserverprovider_export.h"
#include <QObject>

namespace Provider
{

class BLABBYMEDIASERVERPROVIDER_EXPORT MediaServerProviderShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~MediaServerProviderShould() override;
    Q_DISABLE_COPY_MOVE(MediaServerProviderShould);
private Q_SLOTS:
    void give_the_name_of_the_media_server();
    void give_the_icon_of_the_media_server();
};

} // namespace Provider
