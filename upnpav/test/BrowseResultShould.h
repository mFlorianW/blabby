// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

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
    void give_Container_In_The_Result_Field_Of_The_Response_When_Call_Browse_DirectChildren_Finished();
    void give_All_Container_In_The_Result_Field_Of_The_Response_When_Call_Browse_DirectChildren_Finished();
    void give_All_Item_In_The_Result_Field_Of_The_Response_When_Call_Browse_DirectChildren_Finished();
};

} // namespace UPnPAV

#endif // BROWSERESULTSHOULD_H
