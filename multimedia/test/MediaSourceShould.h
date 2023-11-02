// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once
#include <QObject>

namespace Multimedia
{

class MediaSourceShould : public QObject
{
    Q_OBJECT
public:
    MediaSourceShould();
    ~MediaSourceShould() override;
    Q_DISABLE_COPY_MOVE(MediaSourceShould);

private Q_SLOTS:
    void give_the_name_of_media_source();
    void give_a_icon_url_when_set();
};

} // namespace Multimedia
