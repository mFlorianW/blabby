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
#ifndef SERVICECONTROLPOINTDEFINITIONPARSER_H
#define SERVICECONTROLPOINTDEFINITIONPARSER_H

#include "ServiceControlPointDefinition.h"
#include "SCPDStateVariable.h"

#include <QXmlStreamReader>

namespace UPnPAV
{

class ServiceControlPointDefinitionParser final
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

} //namespace UPnPAV

#endif // SERVICECONTROLPOINTDEFINITIONPARSER_H
