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

class GetCurrentConnetionInfoResponseShould : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(GetCurrentConnetionInfoResponseShould)
public:
    using QObject::QObject;
    ~GetCurrentConnetionInfoResponseShould() override = default;

private Q_SLOTS:
    void gives_the_correct_result_for_valid_response();
};

} // namespace UPnPAV
