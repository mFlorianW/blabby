// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef MEDIASERVEROBJECTSHOULD_H
#define MEDIASERVEROBJECTSHOULD_H

#include <QObject>

namespace UPnPAV
{

class MediaServerObjectShould : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void return_The_ID_Set_In_The_Constructor();
    void return_The_ParentId_Set_Passed_In_The_Constructor();
    void return_the_Title_Passed_In_The_Constructor();
    void return_The_TypeClass_Passed_In_The_Constructor();
    void return_the_uri_of_a_playabe_item();
};

} // namespace UPnPAV

#endif // MEDIASERVEROBJECTSHOULD_H
