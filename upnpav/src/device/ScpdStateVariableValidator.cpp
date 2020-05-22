#include "ScpdStateVariableValidator.h"

namespace UPnPAV
{

ScpdStateVariableValidator::ScpdStateVariableValidator(const QString &scpdName,
                                                       const ServiceControlPointDefinition &scpd,
                                                       const QVector<QString> &stateVariableNames)
    : m_scpdName(scpdName)
    , m_scpd(scpd)
    , m_stateVariableNames(stateVariableNames)
{
}

ScpdStateVariableValidator::~ScpdStateVariableValidator()
{
}

bool ScpdStateVariableValidator::validate() noexcept
{
    for(auto &variableName : m_stateVariableNames)
    {
        auto iter = std::find_if(m_scpd.serviceStateTable().begin(),
                                 m_scpd.serviceStateTable().end(),
                                 [=](const SCPDStateVariable &variable)
                                 {
                                    if(variableName == variable.name())
                                    {
                                        return true;
                                    }

                                    return false;
                                 });
        if(iter == m_scpd.serviceStateTable().end())
        {
            m_errorMessage = QString{"%1 SCPD has no %2 variable"}.arg(m_scpdName).arg(variableName);
            return false;
        }
    }

    return true;
}

} //namespace UPnPAV

