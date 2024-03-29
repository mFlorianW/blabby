// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef SERVICECONTROLPOINTDEFINITION_H
#define SERVICECONTROLPOINTDEFINITION_H

#include "SCPDAction.hpp"
#include "SCPDStateVariable.hpp"
#include "blabbyupnpav_export.h"
#include <QExplicitlySharedDataPointer>

namespace UPnPAV
{

struct ServiceControlPointDefinitionData;
class BLABBYUPNPAV_EXPORT ServiceControlPointDefinition
{
public:
    ServiceControlPointDefinition();
    ServiceControlPointDefinition(QString scpdUrl,
                                  QVector<SCPDStateVariable> serviceStateTable,
                                  QVector<SCPDAction> actions = {});

    QString const& scpdUrl() const;

    bool hasStateVariable(QString const& stateVariableName) const noexcept;
    QVector<SCPDStateVariable> const& serviceStateTable() const;

    QVector<SCPDAction> const& actionList() const;
    SCPDAction action(QString const& actionName) const noexcept;

    BLABBYUPNPAV_EXPORT friend bool operator==(ServiceControlPointDefinition const& lhs,
                                               ServiceControlPointDefinition const& rhs);
    BLABBYUPNPAV_EXPORT friend bool operator!=(ServiceControlPointDefinition const& lhs,
                                               ServiceControlPointDefinition const& rhs);

private:
    QExplicitlySharedDataPointer<ServiceControlPointDefinitionData> d;
};

struct ServiceControlPointDefinitionData : public QSharedData
{
    QString mScpdUrl;
    QVector<SCPDStateVariable> mStateVariables;
    QVector<SCPDAction> mActions;

    ServiceControlPointDefinitionData(QString scpdUrl,
                                      QVector<SCPDStateVariable> serviceStateTable,
                                      QVector<SCPDAction> actions)
        : mScpdUrl{std::move(scpdUrl)}
        , mStateVariables{std::move(serviceStateTable)}
        , mActions{std::move(actions)}

    {
    }
};

} // namespace UPnPAV

Q_DECLARE_TYPEINFO(UPnPAV::ServiceControlPointDefinition, Q_RELOCATABLE_TYPE);

#endif // SERVICECONTROLPOINTDEFINITION_H
