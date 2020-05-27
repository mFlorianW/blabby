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
#include "ErrorResult.h"

#include <QXmlStreamReader>
#include <QDebug>

namespace UPnPAV
{

ErrorResult::ErrorResult(const QString &rawMessage)
{
    if(rawMessage.isEmpty())
    {
        return;
    }

    QXmlStreamReader errorReader{rawMessage};

    while(errorReader.readNext() && !errorReader.hasError() && !errorReader.atEnd())
    {
        if(errorReader.isStartElement() && errorReader.name() == "errorCode")
        {
            bool ok = false;
            m_errorCode = errorReader.readElementText().toInt(&ok);

            if(!ok)
            {
                m_errorCode = 0;
            }
        }

        if(errorReader.isStartElement() && errorReader.name() == "errorDescription")
        {
            m_errorDescription = errorReader.readElementText();
        }
    }

    if(errorReader.hasError())
    {
        qInfo() << "Failed to parse error XML result:" << errorReader.errorString();
    }
}

qint32 ErrorResult::errorCode() const noexcept
{
    return m_errorCode;
}

QString ErrorResult::errorDescription() const noexcept
{
    return m_errorDescription;
}

} //namespace UPnPAV
