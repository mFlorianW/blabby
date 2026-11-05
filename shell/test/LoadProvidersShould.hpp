// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <QObject>

namespace Shell
{

class LoadProvidersShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~LoadProvidersShould() override;
    Q_DISABLE_COPY_MOVE(LoadProvidersShould)

private Q_SLOTS:
    void give_every_provider_that_initialized();
    void leave_out_a_provider_that_failed_to_initialize();
    void initialize_every_provider_once();
};

} // namespace Shell
