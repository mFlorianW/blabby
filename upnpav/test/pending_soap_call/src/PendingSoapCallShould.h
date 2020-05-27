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
