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
#include "MediaServerObjectShould.h"
#include "MediaServerObject.h"

#include <QtTest>

namespace UPnPAV
{

void MediaServerObjectShould::return_The_ID_Set_In_The_Constructor()
{
    QString expectedId = "1";
    MediaServerObject mediaServerObject{"1", "", "", ""};

    auto id = mediaServerObject.id();

    QVERIFY2(expectedId == id, QString{"Expected: %1 \nActual:%2"}.arg(expectedId).arg(id).toLocal8Bit());
}

void MediaServerObjectShould::return_The_ParentId_Set_Passed_In_The_Constructor()
{
    QString expectedParentId = "1";
    MediaServerObject mediaServerObject{"", "1", "", ""};

    auto parentId = mediaServerObject.parentId();

    QVERIFY2(expectedParentId == parentId,
             QString{"Expected: %1 \nActual:%2"}.arg(expectedParentId).arg(parentId).toLocal8Bit());
}

void MediaServerObjectShould::return_the_Title_Passed_In_The_Constructor()
{
    QString expectedTitle = "HelloTitle";
    MediaServerObject mediaServerObject{"", "", "HelloTitle", ""};

    auto title = mediaServerObject.title();

    QVERIFY2(expectedTitle == title, QString{"Expected: %1 \nActual: %2"}.arg(expectedTitle).arg(title).toLocal8Bit());
}

void MediaServerObjectShould::return_The_TypeClass_Passed_In_The_Constructor()
{
    QString expectedTypeClass = "typeClass";
    MediaServerObject mediaServerObject{"", "", "", "typeClass"};

    auto typeClass = mediaServerObject.typeClass();

    QVERIFY2(expectedTypeClass == typeClass,
             QString{"Expected: %1 \nActual: %2"}.arg(expectedTypeClass).arg(typeClass).toLocal8Bit());
}

} //namespace UPnPAV

QTEST_MAIN(UPnPAV::MediaServerObjectShould);
