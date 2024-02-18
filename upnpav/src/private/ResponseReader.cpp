// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ResponseReader.hpp"
#include <QDebug>
#include <QXmlStreamReader>

namespace UPnPAV
{

ResponseReader::ResponseReader(QString xmlResponse, ServiceControlPointDefinition scpd, SCPDAction action)
    : mXmlResponse{std::move(xmlResponse)}
    , mScpd{std::move(scpd)}
    , mAction{std::move(action)}
{
}

ResponseReader::~ResponseReader() noexcept = default;

QString const& ResponseReader::response() const noexcept
{
    return mXmlResponse;
}

ResponseReader::ReadResult ResponseReader::read() noexcept
{
    auto xmlReader = QXmlStreamReader{mXmlResponse};
    auto const outArgs = mAction.outArguments();
    auto readResult = ReadResult::Ok;
    while (xmlReader.readNext() && !xmlReader.atEnd() && !xmlReader.hasError()) {
        if (xmlReader.isStartElement()) {
            auto arg = std::find_if(outArgs.cbegin(), outArgs.cend(), [&](SCPDArgument const& arg) {
                return arg.name() == xmlReader.name();
            });

            if (arg != outArgs.cend()) {

                auto const type = dataType(arg->relatedStateVariable());
                if (type == SCPDStateVariable::DataType::Ui4) {
                    readUnsignedIntValue(xmlReader.name().toString(), xmlReader.readElementText());
                } else if (type == SCPDStateVariable::DataType::I4) {
                    readSignedIntValue(xmlReader.name().toString(), xmlReader.readElementText());
                } else if (type == SCPDStateVariable::DataType::String) {
                    auto val = xmlReader.readElementText();
                    Q_EMIT stringValueRead(xmlReader.name().toString(), val, ElementReadResult::Ok);
                } else if (type == SCPDStateVariable::DataType::Ui2) {
                    auto val = static_cast<quint16>(xmlReader.readElementText().toUInt());
                    Q_EMIT readUnsignedInt16Value(xmlReader.name().toString(), val, ElementReadResult::Ok);
                } else {
                    readResult = ReadResult::StateVariableNotFound;
                    break;
                }
            }
        }
    }

    if (xmlReader.hasError()) {
        qCritical() << "Failed GetCurrentConnectionInfo response";
        qCritical() << mXmlResponse;
        qCritical() << "XML Error:" << xmlReader.errorString();
        readResult = ReadResult::Error;
    }
    return readResult;
}

SCPDStateVariable::DataType ResponseReader::dataType(QString const& argName) noexcept
{
    auto const argFound =
        std::find_if(mScpd.serviceStateTable().cbegin(), mScpd.serviceStateTable().cend(), [&](auto const& var) {
            return var.name() == argName;
        });
    if (argFound != mScpd.serviceStateTable().cend()) {
        return argFound->dataType();
    }
    return SCPDStateVariable::DataType::Unknown;
}

void ResponseReader::readUnsignedIntValue(QString const& argName, QString const& rawValue) noexcept
{
    auto ok = false;
    auto result = ElementReadResult::Ok;
    auto const value = rawValue.toUInt(&ok);
    if (not ok) {
        result = ElementReadResult::ConversionError;
    }
    Q_EMIT unsignedIntValueRead(argName, value, result);
}

void ResponseReader::readSignedIntValue(QString const& argName, QString const& rawValue) noexcept
{
    auto ok = false;
    auto result = ElementReadResult::Ok;
    auto const value = rawValue.toInt(&ok);
    if (not ok) {
        result = ElementReadResult::ConversionError;
    }
    Q_EMIT signedIntValueRead(argName, value, result);
}

} // namespace UPnPAV
