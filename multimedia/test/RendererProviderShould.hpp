// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <QObject>

namespace Multimedia
{

class RendererProviderShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~RendererProviderShould() override;
    Q_DISABLE_COPY_MOVE(RendererProviderShould)

private Q_SLOTS:
    void send_find_request_for_media_renderer_on_discover();
    void inform_about_connected_renderer_and_give_the_renderer();
    void inform_about_disconnectd_renderer();
    void inform_about_the_end_of_a_discovery();
    void save_the_remembered_renderers_only_when_they_changed();
};

} // namespace Multimedia
