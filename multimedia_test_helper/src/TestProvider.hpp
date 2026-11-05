// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once
#include "Provider.hpp"

namespace Multimedia::TestHelper
{

class TestProvider : public Multimedia::Provider
{
    Q_OBJECT
    Q_INTERFACES(Multimedia::Provider)
    Q_PLUGIN_METADATA(IID "org.blabby.Provider")
public:
    TestProvider(QString name = QString(""));
    ~TestProvider() override;
    Q_DISABLE_COPY_MOVE(TestProvider)

    bool init() noexcept override;

    Multimedia::MediaSources sources() const noexcept override;

    void createNewSource() noexcept;
    void removeLastSource() noexcept;
    void setInitFails(bool initFails) noexcept;

private:
    bool mInitFails = false;
    QVector<std::shared_ptr<Multimedia::Source>> mSources;
};

} // namespace Multimedia::TestHelper
