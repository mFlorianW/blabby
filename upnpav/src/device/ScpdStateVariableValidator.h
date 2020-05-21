#ifndef SCPDSTATEVARIAABLEVALIDATOR_H
#define SCPDSTATEVARIAABLEVALIDATOR_H

#include "ServiceControlPointDefinition.h"

namespace UPnPAV
{

class ScpdStateVariableValidator final
{
public:
    /**
     * Construst the SCPDSateVariableValidator. After constraction call
     * validate to check for all variables.
     *
     * @param scpdName The name of the SCPD.
     * @param scpd The SCPD itself
     * @param stateVariableNames A list of variable name that shall be checked.
     */
    ScpdStateVariableValidator(const QString &scpdName,
                               const ServiceControlPointDefinition &scpd,
                               const QVector<QString> &stateVariableNames);

    /**
     * Checks if the SCPD contains all the variables given in the variable name vector.
     *
     * @return True all variable names are found.
     */
    bool validate();

    /**
     * Gives the error message when a variable wasn't found. The string will
     * be empty if validate returns true.
     *
     * @return The error message or empty string if validate return true.
     */
    const QString &errorMessage() const noexcept;

private:
    QString m_scpdName;
    QString m_errorMsg;
    ServiceControlPointDefinition m_scpd;
    QVector<QString> m_stateVariableNames;
};

} //namespace UPnPAV

#endif // SCPDSTATEVARIAABLEVALIDATOR_H
