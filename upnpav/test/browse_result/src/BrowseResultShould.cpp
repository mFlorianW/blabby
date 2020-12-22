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
#include "BrowseResultShould.h"
#include "BrowseResult.h"
#include "MediaServerObject.h"
#include "Response.h"

#include <QTest>

namespace UPnPAV
{

BrowseResultShould::BrowseResultShould()
    : QObject()
{
}

void BrowseResultShould::give_The_NumberReturned_Value_Of_The_Response_When_Call_Finished()
{
    QString response = QString{ xmlResponse }.arg("").arg("1").arg("").arg("");
    BrowseResult browseResult{ response };
    quint32 expectedNumber{ 1 };

    auto numberReturned = browseResult.numberReturned();

    QVERIFY2(expectedNumber == numberReturned,
             QString{ "Expected: %1 \nActual: %2" }.arg(expectedNumber).arg(numberReturned).toLocal8Bit());
}

void BrowseResultShould::give_The_TotalMatches_Value_Of_The_Response_When_Call_Finished()
{
    QString response = QString{ xmlResponse }.arg("").arg("").arg("1").arg("");
    BrowseResult browseResult{ response };
    quint32 expectedNumber{ 1 };

    auto totalMatches = browseResult.totalMatches();

    QVERIFY2(expectedNumber == totalMatches,
             QString{ "Expected: %1 \nActual: %2" }.arg(expectedNumber).arg(totalMatches).toLocal8Bit());
}

void BrowseResultShould::give_The_UpdateID_Value_Of_The_Response_When_Call_Finished()
{
    QString response = QString{ xmlResponse }.arg("").arg("").arg("").arg("1");
    BrowseResult browseResult{ response };
    quint32 expectedNumber{ 1 };

    auto updateId = browseResult.updateId();

    QVERIFY2(expectedNumber == updateId, QString{ "Expected: %1 \nActual: %2" }.arg(expectedNumber).arg(updateId).toLocal8Bit());
}

void BrowseResultShould::give_All_Container_In_The_Result_Field_Of_The_Response_When_Call_Browse_DirectChildren_Finished()
{
    QString response = QString{ xmlResponse }.arg(didlOnlyOneContainer).arg("").arg("").arg("");
    BrowseResult browseResult{ response };
    QVector<MediaServerObject> expectedObjects{ MediaServerObject{ "1", "0", "MyMusic", "object.container.storageFolder" } };

    auto objects = browseResult.objects();

    QVERIFY(expectedObjects == objects);
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::BrowseResultShould);
