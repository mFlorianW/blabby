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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QQuickItem>
#include <memory>

namespace Shell
{
class MultiMediaPluginModel;
class MultiMediaPluginSource;
class MainWindowPrivate;

class MainWindow : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MainWindow)

    Q_PROPERTY(Shell::MultiMediaPluginModel *model READ model CONSTANT)

    Q_PROPERTY(QUrl activePluginUrl READ activePluginUrl WRITE setActivePluginUrl NOTIFY activePluginUrlChanged)
    Q_PROPERTY(QString activePluginName READ activePluginName WRITE setActivePluginName NOTIFY activePluginNameChanged)

public:
    MainWindow(MultiMediaPluginModel *model, MultiMediaPluginSource *pluginSource);
    ~MainWindow() override;

    MultiMediaPluginModel *model() const noexcept;
    MultiMediaPluginSource *multiMediaPluginSource() const noexcept;

    QUrl activePluginUrl() const noexcept;
    void setActivePluginUrl(const QUrl &activePluginUrl) noexcept;

    QString activePluginName() const noexcept;
    void setActivePluginName(const QString &activePluginName) noexcept;

    Q_INVOKABLE void activatePlugin(qint32 index) noexcept;

    Q_INVOKABLE void handleBackButtonPressed();

Q_SIGNALS:
    void activePluginUrlChanged();
    void activePluginNameChanged();

private Q_SLOTS:
    void onActiveViewChanged();

private:
    std::unique_ptr<MainWindowPrivate> d;
};

} // namespace Shell

#endif // MAINWINDOW_H
