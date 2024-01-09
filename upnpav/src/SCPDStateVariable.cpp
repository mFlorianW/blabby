// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "SCPDStateVariable.hpp"

namespace UPnPAV
{

SCPDStateVariable::SCPDStateVariable() = default;

SCPDStateVariable::SCPDStateVariable(bool sendEvents,
                                     QString name,
                                     DataType dataType,
                                     QString defaultValue,
                                     QVector<QString> allowedValues,
                                     QString miniumValue,
                                     QString maximumValue,
                                     QString step)
    : d{new SCPDStateVariableData{std::move(sendEvents),
                                  std::move(name),
                                  std::move(dataType),
                                  std::move(defaultValue),
                                  std::move(allowedValues),
                                  std::move(miniumValue),
                                  std::move(maximumValue),
                                  std::move(step)}}
{
}

bool SCPDStateVariable::sendEvents() const
{
    return d->mSendEvents;
}

QString const& SCPDStateVariable::name() const
{
    return d->mName;
}

QString const& SCPDStateVariable::defaultValue() const
{
    return d->mDefaultValue;
}

QVector<QString> const& SCPDStateVariable::allowedValues() const
{
    return d->mAllowedValues;
}

QString const& SCPDStateVariable::miniumValue() const
{
    return d->mMiniumValue;
}

QString const& SCPDStateVariable::maximumValue() const
{
    return d->mMaximumValue;
}

QString const& SCPDStateVariable::stepSize() const
{
    return d->mStep;
}

bool operator==(SCPDStateVariable const& lhs, SCPDStateVariable const& rhs)
{
    if (&lhs == &rhs) {
        return true;
    }

    return ((lhs.d->mSendEvents == rhs.d->mSendEvents) && (lhs.d->mName == rhs.d->mName) &&
            (lhs.d->mDataType == rhs.d->mDataType) && (lhs.d->mDefaultValue == rhs.d->mDefaultValue) &&
            (lhs.d->mAllowedValues == rhs.d->mAllowedValues) && (lhs.d->mMiniumValue == rhs.d->mMiniumValue) &&
            (lhs.d->mMaximumValue == rhs.d->mMaximumValue) && (lhs.d->mStep == rhs.d->mStep));
}

bool operator!=(SCPDStateVariable const& lhs, SCPDStateVariable const& rhs)
{
    return !(lhs == rhs);
}

SCPDStateVariable::DataType SCPDStateVariable::dataType() const
{
    return d->mDataType;
}

} // namespace UPnPAV
