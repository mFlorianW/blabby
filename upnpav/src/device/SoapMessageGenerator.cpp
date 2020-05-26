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
#include "SoapMessageGenerator.h"

#include <QXmlStreamWriter>

namespace UPnPAV
{

SoapMessageGenerator::SoapMessageGenerator()
{
}

QString SoapMessageGenerator::generateXmlMessageBody(const QString
                                                     &actionName, const
                                                     QString &serviceType,
                                                     ArgumentMap args) noexcept
{
    QString result;
    QXmlStreamWriter soapMessabeBody{&result};

    soapMessabeBody.writeStartDocument();
    soapMessabeBody.writeStartElement("s:Envelope");
    soapMessabeBody.writeNamespace("http://schemas.xmlsoap.org/soap/envelope/", "s");
    soapMessabeBody.writeAttribute("s:encodingStyle", "http://schemas.xmlsoap.org/soap/encoding/");
    soapMessabeBody.writeStartElement("s:Body");

    soapMessabeBody.writeStartElement("u:" + actionName);
    soapMessabeBody.writeNamespace(serviceType, "u");

    if(!args.isEmpty())
    {
        for(auto argsIter = args.cbegin(); argsIter != args.cend(); ++argsIter)
        {
            soapMessabeBody.writeTextElement("u:" + argsIter.key(), argsIter.value());
        }
    }

    soapMessabeBody.writeEndElement(/*actionName*/);

    soapMessabeBody.writeEndElement(/*s:Body*/);
    soapMessabeBody.writeEndElement(/*s:Envelope*/);
    soapMessabeBody.writeEndDocument();

    return result.trimmed();
}

} //namespace UPnPAV
