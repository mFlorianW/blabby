// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <QObject>

namespace UPnPAV
{
class MediaRendererShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~MediaRendererShould() override;
    Q_DISABLE_COPY_MOVE(MediaRendererShould)
private Q_SLOTS:
    void throw_an_exception_when_the_device_description_has_no_rendering_control();
    void throw_an_exception_when_the_rendering_control_description_is_not_correct_data();
    void throw_an_exception_when_the_rendering_control_description_is_not_correct();
};
} // namespace UPnPAV
