// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "MainWindow.h"
#include "MainWindow_p.h"
#include "MultimediaPlugin.h"
#include <memory>

namespace Shell
{

MainWindowPrivate::MainWindowPrivate(MultiMediaPluginModel *model, MultiMediaPluginSource *pluginSource)
    : mSource{ pluginSource }
    , mModel{ model }
{
}

MainWindowPrivate::~MainWindowPrivate() = default;

void MainWindowPrivate::loadPlugins()
{
    if((mModel == nullptr) || (mSource == nullptr))
    {
        return;
    }

    mSource->loadPlugins();
    mModel->setPlugins(mSource->plugins());
}

MainWindow::MainWindow(MultiMediaPluginModel *model, MultiMediaPluginSource *pluginSource)
    : QObject()
    , d(std::make_unique<MainWindowPrivate>(model, pluginSource))
{
    Q_ASSERT_X(d->mModel != nullptr, "model", "model parameter shouldn't be nullptr!");
    Q_ASSERT_X(d->mSource != nullptr, "pluginSource", "pluginSource parameter shouldn't be nullptr!");

    d->mSource->loadPlugins();
    d->mModel->setPlugins(pluginSource->plugins());
}

MainWindow::~MainWindow() = default;

MultiMediaPluginModel *MainWindow::model() const noexcept
{
    return d->mModel;
}

MultiMediaPluginSource *MainWindow::multiMediaPluginSource() const noexcept
{
    return d->mSource;
}

QUrl MainWindow::activePluginUrl() const noexcept
{
    return d->mActivePluginUrl;
}

void MainWindow::setActivePluginUrl(const QUrl &activePluginUrl) noexcept
{
    if(d->mActivePluginUrl == activePluginUrl)
    {
        return;
    }

    d->mActivePluginUrl = activePluginUrl;
    Q_EMIT activePluginUrlChanged();
}

QString MainWindow::activePluginName() const noexcept
{
    return d->mActivePluginName;
}

void MainWindow::setActivePluginName(const QString &activePluginName) noexcept
{
    if(d->mActivePluginName == activePluginName)
    {
        return;
    }

    d->mActivePluginName = activePluginName;
    Q_EMIT activePluginNameChanged();
}

void MainWindow::activatePlugin(qint32 index) noexcept
{
    if(d->mModel == nullptr)
    {
        return;
    }

    d->mActivePlugin = d->mModel->plugin(index);
    if(d->mActivePlugin == nullptr)
    {
        return;
    }

    d->mModel->setActivePlugin(index);
    setActivePluginName(d->mActivePlugin->pluginName());
    setActivePluginUrl(d->mActivePlugin->activeView());

    connect(d->mActivePlugin, &PluginCore::MultimediaPlugin::activeViewChanged, this, &MainWindow::onActiveViewChanged);
}

void MainWindow::handleBackButtonPressed()
{
    if(d->mActivePlugin == nullptr)
    {
        return;
    }

    d->mActivePlugin->handleBackButton();
}

void MainWindow::onActiveViewChanged()
{
    if(d->mActivePlugin == nullptr)
    {
        return;
    }

    setActivePluginUrl(d->mActivePlugin->activeView());
}

} // namespace Shell
