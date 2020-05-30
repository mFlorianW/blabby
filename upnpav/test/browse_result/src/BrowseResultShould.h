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
#ifndef BROWSERESULTSHOULD_H
#define BROWSERESULTSHOULD_H

#include <QObject>

namespace UPnPAV
{

class BrowseResultShould : public QObject
{
    Q_OBJECT
public:
    BrowseResultShould();

private Q_SLOTS:
    void give_The_NumberReturned_Value_Of_The_Response_When_Call_Finished();
    void give_The_TotalMatches_Value_Of_The_Response_When_Call_Finished();
    void give_The_UpdateID_Value_Of_The_Response_When_Call_Finished();
    void give_All_Container_In_The_Result_Field_Of_The_Response_When_Call_Browse_DirectChildren_Finished();
};

} //namespace UPnPAV

#endif // BROWSERESULTSHOULD_H
