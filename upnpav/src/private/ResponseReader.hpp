// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#pragma once
#include "SCPDAction.hpp"
#include "ServiceControlPointDefinition.hpp"
#include <QObject>

namespace UPnPAV
{

class ResponseReader : public QObject
{
    Q_OBJECT
public:
    enum class ReadResult
    {
        Ok,
        NoReturnValues,
        ReadWithError,
        StateVariableNotFound,
        Error
    };

    enum class ElementReadResult
    {
        Ok,
        ConversionError,
        Error,
    };

    ResponseReader(QString xmlResponse, ServiceControlPointDefinition scpd, SCPDAction action);
    ~ResponseReader() noexcept override;

    Q_DISABLE_COPY_MOVE(ResponseReader)

    const QString &response() const noexcept;

    ReadResult read() noexcept;

Q_SIGNALS:
    void unsignedIntValueRead(QString const &TokenName,
                              quint32 value,
                              UPnPAV::ResponseReader::ElementReadResult result);
    void signedIntValueRead(QString const &TokenName, qint32 value, UPnPAV::ResponseReader::ElementReadResult result);
    void stringValueRead(QString const &TokenName, QString &value, UPnPAV::ResponseReader::ElementReadResult result);

private:
    SCPDStateVariable::DataType dataType(QString const &argName) noexcept;
    void readUnsignedIntValue(QString const &argName, QString const &rawValue) noexcept;
    void readSignedIntValue(QString const &argName, QString const &rawValue) noexcept;

private:
    QString mXmlResponse;
    ServiceControlPointDefinition mScpd;
    SCPDAction mAction;
    // static QHash<> mSignals;
};

} // namespace UPnPAV
