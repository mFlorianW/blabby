// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once
#include "Provider.hpp"

namespace Multimedia
{

class DummyPlugin : public Provider
{
    Q_OBJECT
    Q_INTERFACES(Multimedia::Provider)
    Q_PLUGIN_METADATA(IID "org.blabby.Provider")
public:
    DummyPlugin();
    ~DummyPlugin() override;
    Q_DISABLE_COPY_MOVE(DummyPlugin)
};

} // namespace Multimedia
