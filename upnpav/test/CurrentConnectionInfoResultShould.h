// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <QObject>
#include <qglobal.h>
#include <qobject.h>
#include <qobjectdefs.h>

namespace UPnPAV
{

class CurrentConnetionInfoResultShould : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(CurrentConnetionInfoResultShould)
public:
    using QObject::QObject;
    ~CurrentConnetionInfoResultShould() override = default;

private Q_SLOTS:
    void gives_the_correct_result_for_valid_response();
};

} // namespace UPnPAV
