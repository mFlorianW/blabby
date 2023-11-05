// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include <QObject>

namespace Multimedia
{

class MediaItemShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~MediaItemShould() override;
    Q_DISABLE_COPY_MOVE(MediaItemShould);
private Q_SLOTS:
    void give_the_type();
    void give_the_main_and_secondary_text();
    void give_the_icon_url();
};

} // namespace Multimedia
