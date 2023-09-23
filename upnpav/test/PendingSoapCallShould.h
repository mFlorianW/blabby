// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef PENDINGREPLYSHOULD_H
#define PENDINGREPLYSHOULD_H

#include <QObject>

namespace UPnPAV
{

class PendingSoapCallShould : public QObject
{
    Q_OBJECT
public:
    PendingSoapCallShould();

private Q_SLOTS:
    void emit_Finished_Signal_When_DataReceived();

    void set_Error_To_True_When_Soap_Call_Finished_With_Error();

    void map_Soap_Error_Codes_To_Enum_When_Call_Finished_data();
    void map_Soap_Error_Codes_To_Enum_When_Call_Finished();

    void give_Error_Description_When_Call_Finished_With_Error();

    void return_An_Object_That_is_Constructable_With_QString_That_Returns_The_Message_When_Call_Finished();
};

} //namespace UPnPAV

#endif // PENDINGREPLYSHOULD_H
