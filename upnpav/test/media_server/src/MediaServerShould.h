#ifndef MEDIASERVERSHOULD_H
#define MEDIASERVERSHOULD_H

#include <QObject>

namespace UPnPAV
{
class ServiceDescription;
class MediaServer;
class ServiceControlPointDefinition;

class MediaServerShould : public QObject
{
    Q_OBJECT
public:
    explicit MediaServerShould();

private:
    MediaServer createMediaServer(const QVector<ServiceDescription> &services,
                                  const QVector<ServiceControlPointDefinition> &scpds);

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

    void throw_An_Exception_When_ConnectionManager_Description_Has_No_SCPD_Url();

    void throw_An_Exception_When_DeviceDescription_Has_No_SCPD_For_ConnectionManager();

    void throw_An_Exception_When_StateVariable_SourceProtocolInfo_Misses_in_ConnectionManager_SCPD();

    void throw_An_Exception_When_StateVariable_SinkProtocolInfo_Misses_in_ConnectionManager_SCPD();
    void throw_Exception_When_StateVar_CurrentConnectionConnectionIDs_Misses_in_ConnectionManager_SCPD();
    void throw_Exception_When_StateVar_A_ARG_TYPE_ConnectionStatus_Misses_in_ConnectionManager_SCPD();
    void throw_Exception_When_StateVar_A_ARG_TYPE_ConnectionManager_Misses_in_ConnectionManager_SCPD();
};

} //namespace UPnPAV

#endif // MEDIASERVERSHOULD_H
