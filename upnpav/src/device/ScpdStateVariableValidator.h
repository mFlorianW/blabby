#ifndef SCPDSTATEVARIAABLEVALIDATOR_H
#define SCPDSTATEVARIAABLEVALIDATOR_H

#include "BaseValidator.h"
#include "ServiceControlPointDefinition.h"

namespace UPnPAV
{

class ScpdStateVariableValidator final : public BaseValidator
{
public:
    /**
     * Construst the SCPDSateVariableValidator. After construction call
     * validate to check for all variables.
     *
     * @param scpdName The name of the SCPD.
     * @param scpd The SCPD itself
     * @param stateVariableNames A list of variable name that shall be checked.
     */
    ScpdStateVariableValidator(const QString &scpdName,
                               const ServiceControlPointDefinition &scpd,
                               const QVector<QString> &stateVariableNames);
    ~ScpdStateVariableValidator() override;

    /**
     * Checks if the SCPD contains all the variables given in the variable name vector.
     *
     * @return True all variable names are found.
     */
    bool validate() noexcept override;

private:
    QString m_scpdName;
    ServiceControlPointDefinition m_scpd;
    QVector<QString> m_stateVariableNames;
};

} //namespace UPnPAV

#endif // SCPDSTATEVARIAABLEVALIDATOR_H
