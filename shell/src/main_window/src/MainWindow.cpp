/**
 ** This file is part of the Blabby project.
 ** Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as
 ** published by the Free Software Foundation, either version 2 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
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
