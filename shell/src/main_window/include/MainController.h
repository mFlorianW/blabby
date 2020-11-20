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
#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QQuickItem>

namespace Shell
{
class MultiMediaPluginModel;
class MultiMediaPluginSource;
class MainControllerPrivate;

class MainController : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MainController)

    Q_PROPERTY(Shell::MultiMediaPluginModel *model READ model WRITE setModel NOTIFY modelChanged)

    Q_PROPERTY(Shell::MultiMediaPluginSource *pluginSource READ multiMediaPluginSource WRITE setMultiMediaPluginSource NOTIFY multiMediaPluginSourceChanged)

public:
    MainController();
    ~MainController() override;

    MultiMediaPluginModel *model() const noexcept;
    void setModel(MultiMediaPluginModel *model) noexcept;

    MultiMediaPluginSource *multiMediaPluginSource() const noexcept;
    void setMultiMediaPluginSource(MultiMediaPluginSource *source) noexcept;

Q_SIGNALS:
    void modelChanged();
    void multiMediaPluginSourceChanged();

private:
    std::unique_ptr<MainControllerPrivate> d;
};

} // namespace Shell

#endif // MAINCONTROLLER_H
