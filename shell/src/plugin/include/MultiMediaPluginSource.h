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
#ifndef MULTIMEDIAPLUGINSOURCE_H
#define MULTIMEDIAPLUGINSOURCE_H

#include <MultiMediaPlugin.h>
#include <QVector>
#include <memory>

namespace Shell
{

class MultiMediaPluginSource : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MultiMediaPluginSource)
public:
    MultiMediaPluginSource() = default;

    virtual ~MultiMediaPluginSource() = default;

    virtual void setPluginQQmlContext(QQmlContext *context) noexcept = 0;
    virtual void loadPlugins() const = 0;

    virtual QVector<std::shared_ptr<PluginCore::MultiMediaPlugin>> plugins() const = 0;
};

} // namespace Shell

#endif // MULTIMEDIAPLUGINSOURCE_H
