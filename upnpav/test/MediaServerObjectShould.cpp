// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MediaServerObjectShould.hpp"
#include "MediaServerObject.hpp"
#include "private/MediaServerObjectBuilder.hpp"

#include <QtTest>

namespace UPnPAV
{

void MediaServerObjectShould::return_The_ID_Set_In_The_Constructor()
{
    QString expectedId = "1";
    MediaServerObject mediaServerObject = MediaServerObjectBuilder{}.setId("1").build();

    auto id = mediaServerObject.id();

    QVERIFY2(expectedId == id, QString{"Expected: %1 \nActual:%2"}.arg(expectedId, id).toLocal8Bit());
}

void MediaServerObjectShould::return_The_ParentId_Set_Passed_In_The_Constructor()
{
    QString expectedParentId = "1";
    MediaServerObject mediaServerObject = MediaServerObjectBuilder{}.setParentId("1").build();

    auto parentId = mediaServerObject.parentId();

    QVERIFY2(expectedParentId == parentId,
             QString{"Expected: %1 \nActual:%2"}.arg(expectedParentId, parentId).toLocal8Bit());
}

void MediaServerObjectShould::return_the_Title_Passed_In_The_Constructor()
{
    QString expectedTitle = "HelloTitle";
    MediaServerObject mediaServerObject = MediaServerObjectBuilder{}.setTitle("HelloTitle").build();

    auto title = mediaServerObject.title();

    QVERIFY2(expectedTitle == title, QString{"Expected: %1 \nActual: %2"}.arg(expectedTitle, title).toLocal8Bit());
}

void MediaServerObjectShould::return_The_TypeClass_Passed_In_The_Constructor()
{
    QString expectedTypeClass = "typeClass";
    MediaServerObject mediaServerObject = MediaServerObjectBuilder{}.setTypeClass("typeClass").build();

    auto typeClass = mediaServerObject.typeClass();

    QVERIFY2(expectedTypeClass == typeClass,
             QString{"Expected: %1 \nActual: %2"}.arg(expectedTypeClass, typeClass).toLocal8Bit());
}

void MediaServerObjectShould::return_the_uri_of_a_playabe_item()
{
    const auto exptectedUri = QStringLiteral("http://test.com/123.mp3");
    const auto mediaServerObject = MediaServerObjectBuilder{}.setPlayUrl(exptectedUri).build();

    QCOMPARE(mediaServerObject.playUrl(), exptectedUri);
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::MediaServerObjectShould);
