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
#include "MainController.h"
#include "MainController_p.h"
#include <memory>

namespace Shell
{

void MainControllerPrivate::loadPlugins()
{
    if((mModel == nullptr) || (mSource == nullptr))
    {
        return;
    }

    mSource->loadPlugins();
    mModel->setPlugins(mSource->plugins());
}

MainController::MainController()
    : QQuickItem()
    , d(std::make_unique<MainControllerPrivate>())
{
}

MainController::~MainController() = default;

MultiMediaPluginModel *MainController::model() const noexcept
{
    return d->mModel;
}

void MainController::setModel(MultiMediaPluginModel *model) noexcept
{
    if(d->mModel == model)
    {
        return;
    }

    d->mModel = model;
    d->loadPlugins();
    Q_EMIT modelChanged();
}

MultiMediaPluginSource *MainController::multiMediaPluginSource() const noexcept
{
    return d->mSource;
}

void MainController::setMultiMediaPluginSource(MultiMediaPluginSource *source) noexcept
{
    if(d->mSource == source)
    {
        return;
    }

    d->mSource = source;
    d->loadPlugins();
    Q_EMIT multiMediaPluginSourceChanged();
}

QUrl MainController::activePluginUrl() const noexcept
{
    return d->mActivePluginUrl;
}

void MainController::setActivePluginUrl(const QUrl &activePluginUrl) noexcept
{
    if(d->mActivePluginUrl == activePluginUrl)
    {
        return;
    }

    d->mActivePluginUrl = activePluginUrl;
    Q_EMIT activePluginUrlChanged();
}

QString MainController::activePluginName() const noexcept
{
    return d->mActivePluginName;
}

void MainController::setActivePluginName(const QString &activePluginName) noexcept
{
    if(d->mActivePluginName == activePluginName)
    {
        return;
    }

    d->mActivePluginName = activePluginName;
    Q_EMIT activePluginNameChanged();
}

void MainController::activatePlugin(qint32 index) noexcept
{
    if(d->mModel == nullptr)
    {
        return;
    }

    auto pluginName = d->mModel->data(d->mModel->index(index), MultiMediaPluginModel::PluginName).value<QString>();
    auto mainQml = d->mModel->data(d->mModel->index(index), MultiMediaPluginModel::PluginQmlUrl).value<QUrl>();
    d->mModel->setActivePlugin(index);
    setActivePluginUrl(mainQml);
    setActivePluginName(pluginName);
}

} // namespace Shell
