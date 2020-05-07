/**
 ** This file is part of the Blabby project.
 ** Copyright 2020 Florian Wessel <florianwessel@gmx.net>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 2 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
#include "ServiceProviderShould.h"

#include <QTest>
#include <QDirIterator>

int main(int argc, char *argv[])
{
    UPnPAV::ServiceProviderShould mediaServerProviderShould;
    qint32 testResult = QTest::qExec(&mediaServerProviderShould, argc, argv);

    if(testResult != 0)
    {
        return testResult;
    }

    //add here further tests

    return 0;
}

