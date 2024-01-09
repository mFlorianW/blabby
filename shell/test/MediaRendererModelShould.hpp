// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "MediaRendererModel.hpp"
#include "ServiceProviderDouble.hpp"
#include <QObject>
#include <memory>

namespace Shell
{
class MediaRendererModelShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~MediaRendererModelShould() override;
    Q_DISABLE_COPY_MOVE(MediaRendererModelShould)

private:
    UPnPAV::Doubles::ServiceProviderDouble* mServiceProvider;
    std::unique_ptr<MediaRendererModel> mModel = nullptr;

private Q_SLOTS:
    void init();
    void give_correct_display_roles_for_the_ui();
    void start_a_mediarenderer_discover_on_init();
    void increase_the_rowCount_on_new_connected_mediarenderer();
    void decrease_the_rowCount_on_disconnected_mediarenderer();
    void give_correct_data_for_the_ui();
    void set_active_renderer_property_correctly();
};

} // namespace Shell
