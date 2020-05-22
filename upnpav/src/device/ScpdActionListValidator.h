#ifndef SCPDACTIONLISTVALIDATOR_H
#define SCPDACTIONLISTVALIDATOR_H

#include "BaseValidator.h"
#include "ServiceControlPointDefinition.h"

namespace UPnPAV
{
/**
 * Validator for action list in the SCPD description.
 */
class ScpdActionListValidator final : public BaseValidator
{
public:
    /**
     * Constructs a SCPDActionListValidator.
     *
     * @param scpdName The name of SCPD. Used for error message.
     * @param scpd The SCPD that shall be checked.
     * @param actionNames The name of the action for which shall be checked.
     */
    ScpdActionListValidator(const QString &scpdName,
                            const ServiceControlPointDefinition &scpd,
                            const QVector<QString> actionNames);
    ~ScpdActionListValidator() override;

    /**
     * Checks if the SCPD contains all the action names.
     *
     * @return True all action names are present, otherwise false.
     */
    bool validate() noexcept override;

private:
    QString m_scpdName;
    ServiceControlPointDefinition m_scpd;
    QVector<QString> m_actionNames;
};

} //namespace UPnPAV

#endif // SCPDACTIONLISTVALIDATOR_H