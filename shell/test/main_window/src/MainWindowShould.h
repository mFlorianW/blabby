// Copyright 2021 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef SHELLSHOULD_H
#define SHELLSHOULD_H

#include <QObject>

namespace Shell
{

class MainWindowShould : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MainWindowShould)
public:
    explicit MainWindowShould(QObject *parent = nullptr);

private Q_SLOTS:
    void forward_back_button_call_to_plugin_when_plugin_active();
};

} // namespace Shell
#endif // SHELLSHOULD_H
