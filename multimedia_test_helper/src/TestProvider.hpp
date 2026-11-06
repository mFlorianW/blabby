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
    QString displayTitle() const noexcept override;
    QList<Multimedia::ProviderSetting> settings() const noexcept override;
    void setSettingValue(QString const& key, QVariant const& value) noexcept override;
    void triggerAction(QString const& key) noexcept override;

    void createNewSource() noexcept;
    void removeLastSource() noexcept;
    void setInitFails(bool initFails) noexcept;
    void setDisplayTitle(QString displayTitle) noexcept;
    void setSettings(QList<Multimedia::ProviderSetting> settings) noexcept;

    /**
     * Gives the setting values the shell passed, in the order it passed them, as key and value.
     */
    QList<std::pair<QString, QVariant>> const& passedValues() const noexcept;

    /**
     * Gives the keys of the actions the shell passed, in the order it passed them.
     */
    QStringList const& triggeredActions() const noexcept;

private:
    bool mInitFails = false;
    QVector<std::shared_ptr<Multimedia::Source>> mSources;
    QString mDisplayTitle;
    QList<Multimedia::ProviderSetting> mSettings;
    QList<std::pair<QString, QVariant>> mPassedValues;
    QStringList mTriggeredActions;
};

} // namespace Multimedia::TestHelper
