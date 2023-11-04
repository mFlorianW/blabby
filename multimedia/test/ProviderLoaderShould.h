// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include <QObject>

namespace Multimedia
{
class ProviderLoaderShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~ProviderLoaderShould() override;
    Q_DISABLE_COPY_MOVE(ProviderLoaderShould)
private Q_SLOTS:
    void provider_should_load_valid_plugins();
};

} // namespace Multimedia
