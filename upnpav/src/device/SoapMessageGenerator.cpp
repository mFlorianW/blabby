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
#include "SCPDAction.h"

#include <QXmlStreamWriter>
#include <QDebug>

namespace UPnPAV
{

SoapMessageGenerator::SoapMessageGenerator()
{
}

QString SoapMessageGenerator::generateXmlMessageBody(const SCPDAction &action,
                                                     const QString &serviceType,
                                                     ArgumentList args) noexcept
{
    QString result;
    QXmlStreamWriter soapMessabeBody{&result};

    soapMessabeBody.writeStartDocument();
    soapMessabeBody.writeStartElement("s:Envelope");
    soapMessabeBody.writeNamespace("http://schemas.xmlsoap.org/soap/envelope/", "s");
    soapMessabeBody.writeAttribute("s:encodingStyle", "http://schemas.xmlsoap.org/soap/encoding/");
    soapMessabeBody.writeStartElement("s:Body");

    soapMessabeBody.writeStartElement("u:" + action.name());
    soapMessabeBody.writeNamespace(serviceType, "u");

    args = putArgumentsInOrderAsInDefiniton(args, action);
    if(!args.isEmpty())
    {
        for(const auto &arg : args)
        {
            soapMessabeBody.writeTextElement("u:" + arg.name, arg.value);
        }
    }

    soapMessabeBody.writeEndElement(/*actionName*/);

    soapMessabeBody.writeEndElement(/*s:Body*/);
    soapMessabeBody.writeEndElement(/*s:Envelope*/);
    soapMessabeBody.writeEndDocument();

    return result.trimmed();
}

QVector<Argument> SoapMessageGenerator::putArgumentsInOrderAsInDefiniton(const QVector<Argument> &args,
                                                                         const SCPDAction &action)
{
    auto inArgs = action.inArguments();
    if(inArgs.size() > args.size())
    {
        qCritical() << "Failed to order in arguments. Given argument size is less then required arguments.";
        qCritical() << "Given in argument size:" << args.size() << "Required in argument size:" << inArgs.size();
        return args;
    }

    QVector<Argument> result{inArgs.size()};
    for(auto index = 0; index < result.size(); ++index)
    {
        auto actionName = inArgs.at(index).name();
        auto actionIter = std::find_if(args.begin(), args.end(),
        [=](const Argument &argument)
        {
            if(argument.name == actionName)
            {
                return true;
            }

            return false;
        });

        if(actionIter == args.end())
        {
            qCritical() << "Argument " << actionName << "not found";
            return args;
        }

        result[index] = std::move(*actionIter);
    }

    return result;
}

} //namespace UPnPAV
