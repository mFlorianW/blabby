// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MultimediaPlugin.h"
#include "MultimediaPlugin_p.h"
#include <QUrl>

namespace PluginCore
{

MultimediaPlugin::MultimediaPlugin()
    : QObject()
    , d(std::make_unique<MultimediaPluginPrivate>())
{
}

MultimediaPlugin::~MultimediaPlugin()
{
}

void MultimediaPlugin::setActiveView(const QUrl &view) noexcept
{
    d->mActiveView = view;
    Q_EMIT activeViewChanged();
}

QUrl MultimediaPlugin::activeView() const noexcept
{
    return d->mActiveView;
}

bool MultimediaPlugin::handleBackButton()
{
    return false;
}

} // namespace PluginCore
