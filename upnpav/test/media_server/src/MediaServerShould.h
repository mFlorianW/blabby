#ifndef MEDIASERVERSHOULD_H
#define MEDIASERVERSHOULD_H

#include <QObject>

namespace UPnPAV
{
class ServiceDescription;
class MediaServer;
class ServiceControlPointDefinition;
class SCPDStateVariable;
class SCPDAction;


class MediaServerShould : public QObject
{
    Q_OBJECT
public:
    explicit MediaServerShould();

private:
    MediaServer createMediaServer(const QVector<ServiceDescription> &services,
                                  const QVector<ServiceControlPointDefinition> &scpds);

    ServiceControlPointDefinition createConnectionManagerSCPDWithoutStateVariable(const SCPDStateVariable &variable);

    ServiceControlPointDefinition createConnectionManagerSCPDWithoutAction(const SCPDAction &action);

    ServiceControlPointDefinition createContentDirectorySCPDWithoutStateVariable(const SCPDStateVariable &variable);

    ServiceControlPointDefinition createContentDirectorySCPDWithoutAction(const SCPDAction &action);

private Q_SLOTS:
    /**
     * @test The media server shall throw an exception when the ContentDirectory Service description
     * is missing.
     */
    void throw_An_Exception_When_DeviceDescription_Has_No_ContentDirectory_Service_Description();

    /**
     * @test The media server shall throw an exception when the ContentDirectory service has
     * no event URL set.
     */
    void throw_An_Exception_When_ContentDirectory_Description_Has_No_Url();

    /**
     * @test The media server shall throw an exception when the ContentDirectory service has
     * no control URL set.
     */
    void throw_An_Exception_When_ContentDirectory_Description_Has_No_Control_Url();

    /**
     * @test The media server shall throw an exception when the ContentDirectory service
     * has no service ID set.
     */
    void throw_An_Exception_When_ContentDirectory_Description_Has_No_ServiceId();

    /**
     * @test The media server shall throw an exception when the ContentDirectory service
     * has no SCPD URL set.
     */
    void throw_An_Exception_When_ContentDirectory_Description_Has_No_SCPD_Url();

    /**
     * @test The media sever shall throw an exception when the ConnectionManager Service description
     * is missing.
     */
    void throw_An_Exception_When_DeviceDescription_Has_No_ConnectionManagerDescription();

    /**
     * @test The media server shall throw an exception when the ConnectionManager service
     * has no event URL set.
     */
    void throw_An_Exception_When_ConnectionManager_Description_Has_No_Event_Url();

    /**
     * @test The media server shall throw an exception when the ConnectionMangager service
     * has no control URL set.
     */
    void throw_An_Exception_When_ConnectionManager_Description_Has_No_Control_Url();

    /**
     * @test The media server shall throw an exception when the ConnectionManager service
     * has no service ID set.
     */
    void throw_An_Exception_When_ConnectionManager_Description_Has_ServiceId();

    /**
     * @test The media server shall throw an exception when the ConnectionManager service has no
     * SCPD url set.
     */
    void throw_An_Exception_When_ConnectionManager_Description_Has_No_SCPD_Url();

    /**
     * @test The media server shall throw an exception when the DeviceDescription
     * has no SCPD for the ConnectionManager.
     */
    void throw_An_Exception_When_DeviceDescription_Has_No_SCPD_For_ConnectionManager();

    void throw_Exception_When_StateVariable_Misses_In_ConnectionManager_SCPD_data();
    /**
     * @test The media server shall throw an exception on construction when the ConnectionManager SCPD misses one
     * of the minimum required state variables in the device description.
     */
    void throw_Exception_When_StateVariable_Misses_In_ConnectionManager_SCPD();

    void Throw_Exception_When_Action_Misses_in_ConnectionManager_SCPD_data();
    /**
     * @test The media server shall throw an exception on constructuion when the ConnectionManager SCPD misses one
     * of the minimum required actions in the device description.
     */
    void Throw_Exception_When_Action_Misses_in_ConnectionManager_SCPD();

    void throw_Exception_When_StateVariable_Misses_In_ContentDirectory_SCPD_data();
    /**
     * @test The media server shall throw an exception on construction when the ContentDirectory SCPD misses one
     * of the minimum required state variables in the device description.
     */
    void throw_Exception_When_StateVariable_Misses_In_ContentDirectory_SCPD();

    void throw_Exception_When_Action_Misses_in_ContentDirectory_SCPD_data();
    /**
     * @test The media server shall throw an exception on constructuion when the ContentDirectory SCPD misses one
     * of the minimum required actions in the device description.
     */
    void throw_Exception_When_Action_Misses_in_ContentDirectory_SCPD();
};

} //namespace UPnPAV

#endif // MEDIASERVERSHOULD_H
