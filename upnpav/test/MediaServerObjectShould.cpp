// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MediaServerObjectShould.hpp"
#include "MediaServerObject.hpp"

#include <QtTest>

namespace UPnPAV
{

void MediaServerObjectShould::return_The_ID_Set_In_The_Constructor()
{
    QString expectedId = "1";
    MediaServerObject mediaServerObject{"1", "", "", ""};

    auto id = mediaServerObject.id();

    QVERIFY2(expectedId == id, QString{"Expected: %1 \nActual:%2"}.arg(expectedId, id).toLocal8Bit());
}

void MediaServerObjectShould::return_The_ParentId_Set_Passed_In_The_Constructor()
{
    QString expectedParentId = "1";
    MediaServerObject mediaServerObject{"", "1", "", ""};

    auto parentId = mediaServerObject.parentId();

    QVERIFY2(expectedParentId == parentId,
             QString{"Expected: %1 \nActual:%2"}.arg(expectedParentId, parentId).toLocal8Bit());
}

void MediaServerObjectShould::return_the_Title_Passed_In_The_Constructor()
{
    QString expectedTitle = "HelloTitle";
    MediaServerObject mediaServerObject{"", "", "HelloTitle", ""};

    auto title = mediaServerObject.title();

    QVERIFY2(expectedTitle == title, QString{"Expected: %1 \nActual: %2"}.arg(expectedTitle, title).toLocal8Bit());
}

void MediaServerObjectShould::return_The_TypeClass_Passed_In_The_Constructor()
{
    QString expectedTypeClass = "typeClass";
    MediaServerObject mediaServerObject{"", "", "", "typeClass"};

    auto typeClass = mediaServerObject.typeClass();

    QVERIFY2(expectedTypeClass == typeClass,
             QString{"Expected: %1 \nActual: %2"}.arg(expectedTypeClass, typeClass).toLocal8Bit());
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::MediaServerObjectShould);
