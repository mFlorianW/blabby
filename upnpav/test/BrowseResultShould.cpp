// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "BrowseResultShould.h"
#include "BrowseResult.h"
#include "ContentDirectoryActions.h"
#include "Descriptions.h"
#include "MediaServerObject.h"
#include "Response.h"

#include <QTest>

namespace UPnPAV
{

namespace
{
BrowseResult createBrowseResult(QString const &response)
{
    return BrowseResult{response, validContentDirectorySCPD, Browse};
}
} // namespace

BrowseResultShould::BrowseResultShould()
    : QObject()
{
}

void BrowseResultShould::give_The_NumberReturned_Value_Of_The_Response_When_Call_Finished()
{
    QString response = QString{xmlResponse}.arg("").arg("1").arg("").arg("");
    auto browseResult = createBrowseResult(response);
    quint32 expectedNumber{1};

    auto numberReturned = browseResult.numberReturned();

    QVERIFY2(expectedNumber == numberReturned,
             QString{"Expected: %1 \nActual: %2"}.arg(expectedNumber).arg(numberReturned).toLocal8Bit());
}

void BrowseResultShould::give_The_TotalMatches_Value_Of_The_Response_When_Call_Finished()
{
    QString response = QString{xmlResponse}.arg("").arg("").arg("1").arg("");
    auto browseResult = createBrowseResult(response);
    quint32 expectedNumber{1};

    auto totalMatches = browseResult.totalMatches();

    QVERIFY2(expectedNumber == totalMatches,
             QString{"Expected: %1 \nActual: %2"}.arg(expectedNumber).arg(totalMatches).toLocal8Bit());
}

void BrowseResultShould::give_The_UpdateID_Value_Of_The_Response_When_Call_Finished()
{
    QString response = QString{xmlResponse}.arg("").arg("").arg("").arg("1");
    auto browseResult = createBrowseResult(response);
    quint32 expectedNumber{1};

    auto updateId = browseResult.updateId();

    QVERIFY2(expectedNumber == updateId,
             QString{"Expected: %1 \nActual: %2"}.arg(expectedNumber).arg(updateId).toLocal8Bit());
}

void BrowseResultShould::give_Container_In_The_Result_Field_Of_The_Response_When_Call_Browse_DirectChildren_Finished()
{
    QString response = QString{xmlResponse}.arg(didlOnlyOneContainer).arg("").arg("").arg("");
    auto browseResult = createBrowseResult(response);
    QVector<MediaServerObject> expectedObjects{
        MediaServerObject{"1", "0", "MyMusic", "object.container.storageFolder"}};

    auto objects = browseResult.objects();

    QVERIFY(expectedObjects == objects);
}

void BrowseResultShould::
    give_All_Container_In_The_Result_Field_Of_The_Response_When_Call_Browse_DirectChildren_Finished()
{
    QString response = QString{xmlResponse}.arg(didlOnlyTwoContainer, (""), (""), (""));
    auto browseResult = createBrowseResult(response);
    QVector<MediaServerObject> expectedObjects{
        MediaServerObject{"1", "0", "MyMusic0", "object.container.storageFolder"},
        MediaServerObject{"2", "0", "MyMusic1", "object.container.storageFolder"}};

    auto objects = browseResult.objects();

    QVERIFY(expectedObjects == objects);
}

void BrowseResultShould::give_All_Item_In_The_Result_Field_Of_The_Response_When_Call_Browse_DirectChildren_Finished()
{
    QString response = QString{xmlResponse}.arg(didlOnlyOneItem, "1", "1", "33");
    auto browseResult = createBrowseResult(response);
    QVector<MediaServerObject> expectedObjects{
        MediaServerObject{"64$0$0", "64$0", "Dekmantel Boiler Room 2016 -", "object.item.audioItem.musicTrack"}};

    auto objects = browseResult.objects();

    QVERIFY(expectedObjects == objects);
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::BrowseResultShould);
