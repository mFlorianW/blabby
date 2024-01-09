// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ServiceControlPointDefinitionParser.hpp"
#include "ParsingError.hpp"

#include <QDebug>
#include <utility>

namespace UPnPAV
{

ServiceControlPointDefinitionParser::ServiceControlPointDefinitionParser() = default;

ServiceControlPointDefinitionParser::ServiceControlPointDefinitionParser(QString url)
    : m_url(std::move(url))
{
}

void ServiceControlPointDefinitionParser::parseServiceControlPointDefinition(QString const& scpd)
{
    m_streamReader.addData(scpd);

    while (!m_streamReader.atEnd() && !m_streamReader.hasError()) {
        (void)m_streamReader.readNext();
        if ((m_streamReader.isStartDocument()) || (m_streamReader.isEndDocument()) || (m_streamReader.isEndElement())) {
            continue;
        }

        if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("actionList"))) {
            m_actions = parseActionList();
        }

        if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("serviceStateTable"))) {
            m_stateVariablies = parseServiceStateTable();
        }
    }

    if (m_streamReader.hasError()) {
        qInfo() << m_streamReader.errorString();
        throw ParsingError{""};
    }
}

ServiceControlPointDefinition ServiceControlPointDefinitionParser::serviceControlPointDefinition() const noexcept
{
    QVector<SCPDAction> actions;
    for (auto const& action : m_actions) {
        QVector<SCPDArgument> arguments;
        for (auto const& argument : action.arguments) {
            arguments.append(SCPDArgument{argument.name, argument.direction, argument.relatedStateVariable});
        }

        actions.append(SCPDAction{action.name, arguments});
    }

    QVector<SCPDStateVariable> variables;
    for (auto const& variable : m_stateVariablies) {
        variables.append(SCPDStateVariable{variable.sendEvent,
                                           variable.name,
                                           variable.dataType,
                                           variable.defaultValue,
                                           variable.allowedValues,
                                           variable.minimumValue,
                                           variable.maxiumValue,
                                           variable.step});
    }

    return ServiceControlPointDefinition{m_url, variables, actions};
}

QVector<ServiceControlPointDefinitionParser::TempSCPDAction> ServiceControlPointDefinitionParser::parseActionList()
{
    QVector<TempSCPDAction> result;

    while ((!(m_streamReader.isEndElement() && (m_streamReader.name() == QStringLiteral("actionList"))) &&
            !m_streamReader.hasError() && !m_streamReader.atEnd())) {
        (void)m_streamReader.readNext();

        if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("action"))) {
            result.append(parseAction());
        }
    }

    return result;
}

ServiceControlPointDefinitionParser::TempSCPDAction ServiceControlPointDefinitionParser::parseAction()
{
    TempSCPDAction action;

    while ((!(m_streamReader.isEndElement() && (m_streamReader.name() == QStringLiteral("action"))) &&
            !m_streamReader.hasError() && !m_streamReader.atEnd())) {
        (void)m_streamReader.readNext();

        if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("name"))) {
            action.name = m_streamReader.readElementText();
        } else if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("argumentList"))) {
            action.arguments = parseArguments();
        }
    }

    return action;
}

QVector<ServiceControlPointDefinitionParser::TempArgument> ServiceControlPointDefinitionParser::parseArguments()
{
    QVector<TempArgument> arguments;

    while ((!(m_streamReader.isEndElement() && (m_streamReader.name() == QStringLiteral("argumentList"))) &&
            !m_streamReader.hasError() && !m_streamReader.atEnd())) {
        (void)m_streamReader.readNext();

        if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("argument"))) {
            arguments.append(parseArgument());
        }
    }

    return arguments;
}

ServiceControlPointDefinitionParser::TempArgument ServiceControlPointDefinitionParser::parseArgument()
{
    TempArgument argument;

    while ((!(m_streamReader.isEndElement() && (m_streamReader.name() == QStringLiteral("argument"))) &&
            !m_streamReader.hasError() && !m_streamReader.atEnd())) {
        (void)m_streamReader.readNext();

        if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("name"))) {
            argument.name = m_streamReader.readElementText();
        } else if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("direction"))) {
            argument.direction = convertStringToDirection(m_streamReader.readElementText());
        } else if (m_streamReader.isStartElement() &&
                   (m_streamReader.name() == QStringLiteral("relatedStateVariable"))) {
            argument.relatedStateVariable = m_streamReader.readElementText();
        }
    }

    return argument;
}

QVector<ServiceControlPointDefinitionParser::TempSCPDStateVariable> ServiceControlPointDefinitionParser::
    parseServiceStateTable()
{
    QVector<TempSCPDStateVariable> result;

    while ((!(m_streamReader.isEndElement() && (m_streamReader.name() == QStringLiteral("serviceStateTable"))) &&
            !m_streamReader.hasError() && !m_streamReader.atEnd())) {
        (void)m_streamReader.readNext();

        if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("stateVariable"))) {
            result.append(parseStateVariable());
        }
    }

    return result;
}

ServiceControlPointDefinitionParser::TempSCPDStateVariable ServiceControlPointDefinitionParser::parseStateVariable()
{
    TempSCPDStateVariable stateVariable;

    while ((!(m_streamReader.isEndElement() && (m_streamReader.name() == QStringLiteral("stateVariable"))) &&
            !m_streamReader.hasError() && !m_streamReader.atEnd())) {
        (void)m_streamReader.readNext();

        if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("name"))) {
            stateVariable.name = m_streamReader.readElementText();
        } else if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("dataType"))) {
            stateVariable.dataType = parseStateVariableDataType(m_streamReader.readElementText());
        } else if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("defaultValue"))) {
            stateVariable.defaultValue = m_streamReader.readElementText();
        } else if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("allowedValueList"))) {
            stateVariable.allowedValues = parseAllowedValueList();
        } else if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("allowedValueRange"))) {
            auto allowedRange = parseAllowedValueRange();

            stateVariable.minimumValue = allowedRange.minimum;
            stateVariable.maxiumValue = allowedRange.maximum;
            stateVariable.step = allowedRange.step;
        }
    }

    return stateVariable;
}

SCPDStateVariable::DataType ServiceControlPointDefinitionParser::parseStateVariableDataType(QString const& dataType)
{
    if (dataType == "ui1") {
        return SCPDStateVariable::DataType::Ui1;
    } else if (dataType == "ui2") {
        return SCPDStateVariable::DataType::Ui2;
    } else if (dataType == "ui4") {
        return SCPDStateVariable::DataType::Ui4;
    } else if (dataType == "i1") {
        return SCPDStateVariable::DataType::I1;
    } else if (dataType == "i2") {
        return SCPDStateVariable::DataType::I2;
    } else if (dataType == "i4") {
        return SCPDStateVariable::DataType::I4;
    } else if (dataType == "r4") {
        return SCPDStateVariable::DataType::R4;
    } else if (dataType == "r8") {
        return SCPDStateVariable::DataType::R8;
    } else if (dataType == "char") {
        return SCPDStateVariable::DataType::Char;
    } else if (dataType == "string") {
        return SCPDStateVariable::DataType::String;
    } else if (dataType == "date") {
        return SCPDStateVariable::DataType::Date;
    } else if (dataType == "dateTime") {
        return SCPDStateVariable::DataType::DateTime;
    } else if (dataType == "dateTime.tz") {
        return SCPDStateVariable::DataType::DateTimeTz;
    } else if (dataType == "time") {
        return SCPDStateVariable::DataType::Time;
    } else if (dataType == "time.tz") {
        return SCPDStateVariable::DataType::TimeTz;
    } else if (dataType == "boolean") {
        return SCPDStateVariable::DataType::Bool;
    } else if (dataType == "uri") {
        return SCPDStateVariable::DataType::Uri;
    } else if (dataType == "uuid") {
        return SCPDStateVariable::DataType::Uuid;
    }

    return SCPDStateVariable::DataType::Unknown;
}

QVector<QString> ServiceControlPointDefinitionParser::parseAllowedValueList()
{
    QVector<QString> allowedValues;

    while ((!(m_streamReader.isEndElement() && (m_streamReader.name() == QStringLiteral("allowedValueList"))) &&
            !m_streamReader.hasError() && !m_streamReader.atEnd())) {
        (void)m_streamReader.readNext();

        if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("allowedValue"))) {
            allowedValues.append(m_streamReader.readElementText());
        }
    }

    return allowedValues;
}

ServiceControlPointDefinitionParser::SCPDAllowedValueRange ServiceControlPointDefinitionParser::parseAllowedValueRange()
{
    SCPDAllowedValueRange allowedRange;

    while ((!(m_streamReader.isEndElement() && (m_streamReader.name() == QStringLiteral("allowedValueRange"))) &&
            !m_streamReader.hasError() && !m_streamReader.atEnd())) {
        (void)m_streamReader.readNext();

        if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("minimum"))) {
            allowedRange.minimum = m_streamReader.readElementText();
        } else if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("maximum"))) {
            allowedRange.maximum = m_streamReader.readElementText();
        } else if (m_streamReader.isStartElement() && (m_streamReader.name() == QStringLiteral("step"))) {
            allowedRange.step = m_streamReader.readElementText();
        }
    }

    return allowedRange;
}

SCPDArgument::Direction ServiceControlPointDefinitionParser::convertStringToDirection(QString const& direction)
{
    if (direction == QStringLiteral("in")) {
        return SCPDArgument::Direction::In;
    }

    return SCPDArgument::Direction::Out;
}

} // Namespace UPnPAV
