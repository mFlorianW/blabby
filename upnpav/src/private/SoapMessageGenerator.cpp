// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "SoapMessageGenerator.hpp"
#include "SCPDAction.hpp"
#include <QDebug>
#include <QXmlStreamWriter>

namespace UPnPAV
{

SoapMessageGenerator::SoapMessageGenerator() = default;

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
    if (!args.isEmpty()) {
        for (const auto &arg : args) {
            soapMessabeBody.writeTextElement(arg.name, arg.value);
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
    if (inArgs.size() > args.size()) {
        qCritical() << "Failed to order in arguments. Given argument size is less then required arguments.";
        qCritical() << "Given in argument size:" << args.size() << "Required in argument size:" << inArgs.size();
        return args;
    }

    QVector<Argument> result{inArgs.size()};
    for (auto index = 0; index < result.size(); ++index) {
        auto actionName = inArgs.at(index).name();
        auto actionIter = std::find_if(args.begin(), args.end(), [=](const Argument &argument) {
            if (argument.name == actionName) {
                return true;
            }

            return false;
        });

        if (actionIter == args.end()) {
            qCritical() << "Argument " << actionName << "not found";
            return args;
        }

        result[index] = std::move(*actionIter);
    }

    return result;
}

} // namespace UPnPAV
