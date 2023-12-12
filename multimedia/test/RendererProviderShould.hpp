// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
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
};

} // namespace Multimedia
