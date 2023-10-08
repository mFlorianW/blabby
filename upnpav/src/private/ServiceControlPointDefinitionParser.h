// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef SERVICECONTROLPOINTDEFINITIONPARSER_H
#define SERVICECONTROLPOINTDEFINITIONPARSER_H

#include "SCPDStateVariable.h"
#include "ServiceControlPointDefinition.h"
#include "blabbyupnpav_export.h"
#include <QXmlStreamReader>

namespace UPnPAV
{

class BLABBYUPNPAV_EXPORT ServiceControlPointDefinitionParser final
{
public:
    ServiceControlPointDefinitionParser();

    ServiceControlPointDefinitionParser(const QString &url);

    void parseServiceControlPointDefinition(const QString &scpd);

    ServiceControlPointDefinition serviceControlPointDefinition() const noexcept;

private:
    struct TempArgument
    {
        QString name;
        SCPDArgument::Direction direction;
        QString relatedStateVariable;
    };

    struct TempSCPDAction
    {
        QString name;
        QVector<ServiceControlPointDefinitionParser::TempArgument> arguments;
    };

    struct SCPDAllowedValueRange
    {
        QString minimum;
        QString maximum;
        QString step;
    };

    struct TempSCPDStateVariable
    {
        bool sendEvent{false};
        QString name;
        SCPDStateVariable::DataType dataType;
        QString defaultValue;
        QVector<QString> allowedValues;
        QString minimumValue;
        QString maxiumValue;
        QString step;
    };

private:
    QVector<TempSCPDAction> parseActionList();
    TempSCPDAction parseAction();
    QVector<TempArgument> parseArguments();
    TempArgument parseArgument();

    QVector<TempSCPDStateVariable> parseServiceStateTable();
    TempSCPDStateVariable parseStateVariable();
    SCPDStateVariable::DataType parseStateVariableDataType(const QString &dataType);
    QVector<QString> parseAllowedValueList();
    SCPDAllowedValueRange parseAllowedValueRange();

    static SCPDArgument::Direction convertStringToDirection(const QString &direction);

private:
    QString m_url;

    QXmlStreamReader m_streamReader;

    QVector<TempSCPDAction> m_actions;
    QVector<TempSCPDStateVariable> m_stateVariablies;
};

} // namespace UPnPAV

#endif // SERVICECONTROLPOINTDEFINITIONPARSER_H
