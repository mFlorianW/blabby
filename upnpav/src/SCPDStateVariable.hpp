// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef SCPDSTATEVARIABLE_H
#define SCPDSTATEVARIABLE_H

#include "blabbyupnpav_export.h"
#include <QExplicitlySharedDataPointer>
#include <QString>
#include <QVector>

namespace UPnPAV
{

struct SCPDStateVariableData;
class BLABBYUPNPAV_EXPORT SCPDStateVariable
{
public:
    enum DataType
    {
        Unknown,

        Ui1,
        Ui2,
        Ui4,

        I1,
        I2,
        I4,
        // int TODO: handle this type

        R4,
        R8,
        // number TODO: handle this type
        // fixed TODO: handle this type
        // float TODO: TODO: handle this type

        Char,
        String,

        Date,
        DateTime,
        DateTimeTz,
        Time,
        TimeTz,

        Bool,

        // Bin.base64 TODO: handle this type
        // Bin.hex TODO: handle this type

        Uri,
        Uuid,
    };

    SCPDStateVariable();

    SCPDStateVariable(bool sendEvents,
                      QString name,
                      DataType dataType,
                      QString defaultValue = {},
                      QVector<QString> allowedValues = {},
                      QString miniumValue = {},
                      QString maximumValue = {},
                      QString step = {});

    bool sendEvents() const;

    QString const& name() const;

    DataType dataType() const;

    QString const& defaultValue() const;

    QVector<QString> const& allowedValues() const;

    QString const& miniumValue() const;

    QString const& maximumValue() const;

    QString const& stepSize() const;

    BLABBYUPNPAV_EXPORT friend bool operator==(SCPDStateVariable const& lhs, SCPDStateVariable const& rhs);
    BLABBYUPNPAV_EXPORT friend bool operator!=(SCPDStateVariable const& lhs, SCPDStateVariable const& rhs);

private:
    QExplicitlySharedDataPointer<SCPDStateVariableData> d;
};

struct SCPDStateVariableData : public QSharedData
{
    bool mSendEvents{false};
    QString mName;
    SCPDStateVariable::DataType mDataType;
    QString mDefaultValue;
    QVector<QString> mAllowedValues;
    QString mMiniumValue;
    QString mMaximumValue;
    QString mStep;

    SCPDStateVariableData(bool sendEvents,
                          QString name,
                          SCPDStateVariable::DataType dataType,
                          QString defaultValue,
                          QVector<QString> allowedValues,
                          QString miniumValue,
                          QString maximumValue,
                          QString step)
        : mSendEvents{std::move(sendEvents)}
        , mName{std::move(name)}
        , mDataType{std::move(dataType)}
        , mDefaultValue{std::move(defaultValue)}
        , mAllowedValues{std::move(allowedValues)}
        , mMiniumValue{std::move(miniumValue)}
        , mMaximumValue{std::move(maximumValue)}
        , mStep{std::move(step)}
    {
    }
};

} // namespace UPnPAV

Q_DECLARE_TYPEINFO(UPnPAV::SCPDStateVariable, Q_RELOCATABLE_TYPE);

#endif // SCPDSTATEVARIABLE_H
