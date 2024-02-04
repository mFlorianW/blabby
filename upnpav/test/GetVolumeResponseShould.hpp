// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once
#include <QObject>

namespace UPnPAV
{

class GetVolumeResponseShould : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(GetVolumeResponseShould)
public:
    using QObject::QObject;
    ~GetVolumeResponseShould() override = default;

private Q_SLOTS:
    void gives_the_correct_volume_for_valid_response();
};

} // namespace UPnPAV
