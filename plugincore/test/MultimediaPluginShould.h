// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef MULTIMEDIAPLUGINSHOULD_H
#define MULTIMEDIAPLUGINSHOULD_H

#include <QObject>

namespace PluginCore
{

class MultimediaPluginShould : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MultimediaPluginShould)
public:
    explicit MultimediaPluginShould(QObject *parent = nullptr);
    ~MultimediaPluginShould() override = default;

private Q_SLOTS:
    void be_able_to_set_a_active_view();
};

} // namespace PluginCore

#endif // MULTIMEDIAPLUGINSHOULD_H
