// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
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
    MediaServerObject mediaServerObject = MediaServerObjectBuilder{}.withId("1").build();

    auto id = mediaServerObject.id();

    QVERIFY2(expectedId == id, QString{"Expected: %1 \nActual:%2"}.arg(expectedId, id).toLocal8Bit());
}

void MediaServerObjectShould::return_The_ParentId_Set_Passed_In_The_Constructor()
{
    QString expectedParentId = "1";
    MediaServerObject mediaServerObject = MediaServerObjectBuilder{}.withParentId("1").build();

    auto parentId = mediaServerObject.parentId();

    QVERIFY2(expectedParentId == parentId,
             QString{"Expected: %1 \nActual:%2"}.arg(expectedParentId, parentId).toLocal8Bit());
}

void MediaServerObjectShould::return_the_Title_Passed_In_The_Constructor()
{
    QString expectedTitle = "HelloTitle";
    MediaServerObject mediaServerObject = MediaServerObjectBuilder{}.withTitle("HelloTitle").build();

    auto title = mediaServerObject.title();

    QVERIFY2(expectedTitle == title, QString{"Expected: %1 \nActual: %2"}.arg(expectedTitle, title).toLocal8Bit());
}

void MediaServerObjectShould::return_The_TypeClass_Passed_In_The_Constructor()
{
    QString expectedTypeClass = "typeClass";
    MediaServerObject mediaServerObject = MediaServerObjectBuilder{}.withTypeClass("typeClass").build();

    auto typeClass = mediaServerObject.typeClass();

    QVERIFY2(expectedTypeClass == typeClass,
             QString{"Expected: %1 \nActual: %2"}.arg(expectedTypeClass, typeClass).toLocal8Bit());
}

void MediaServerObjectShould::return_the_uri_of_a_playabe_item()
{
    auto const exptectedUri = QStringLiteral("http://test.com/123.mp3");
    auto const mediaServerObject = MediaServerObjectBuilder{}.withPlayUrl(exptectedUri).build();

    QCOMPARE(mediaServerObject.playUrl(), exptectedUri);
}

void MediaServerObjectShould::return_the_supported_protocols()
{
    auto const expProtocols =
        QVector<Protocol>{Protocol::create(QStringLiteral("http-get:*:audio/mpeg")).value_or(Protocol{})};
    auto const mediaServerObject = MediaServerObjectBuilder{}.withSupportedProtocols(expProtocols).build();

    QCOMPARE(mediaServerObject.supportedProtocols(), expProtocols);
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::MediaServerObjectShould);
