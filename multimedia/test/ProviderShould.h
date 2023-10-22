// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once

#include <QObject>

namespace Multimedia
{

class ProviderShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~ProviderShould() override;
    Q_DISABLE_COPY_MOVE(ProviderShould)
private Q_SLOTS:
    void give_the_name_of_the_provider();
    void give_the_version_of_the_provider();
};

} // namespace Multimedia
