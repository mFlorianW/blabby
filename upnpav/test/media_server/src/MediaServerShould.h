/**
 ** This file is part of the Blabby project.
 ** Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as
 ** published by the Free Software Foundation, either version 2 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
#ifndef MEDIASERVERSHOULD_H
#define MEDIASERVERSHOULD_H

#include <QObject>
#include <QSharedPointer>

namespace UPnPAV
{
class ServiceDescription;
class MediaServer;
class ServiceControlPointDefinition;
class SCPDStateVariable;
class SCPDAction;
class DeviceDescription;
class SoapMessageTransmitterDouble;

class MediaServerShould : public QObject
{
    Q_OBJECT
public:
    explicit MediaServerShould();

private:
    MediaServer createMediaServer(const QVector<ServiceDescription> &services, const QVector<ServiceControlPointDefinition> &scpds);

    ServiceControlPointDefinition createConnectionManagerSCPDWithoutStateVariable(const SCPDStateVariable &variable);

    ServiceControlPointDefinition createConnectionManagerSCPDWithoutAction(const SCPDAction &action);

    ServiceControlPointDefinition createContentDirectorySCPDWithoutStateVariable(const SCPDStateVariable &variable);

    ServiceControlPointDefinition createContentDirectorySCPDWithoutAction(const SCPDAction &action);

    MediaServer createMediaServer(DeviceDescription &deviceDescription);

    QSharedPointer<SoapMessageTransmitterDouble> m_soapMessageTransmitter;

private Q_SLOTS:

    void init();

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

    void shall_Send_The_SOAP_Message_When_Calling_GetSortCapabilities();

    void shall_Send_The_SOAP_Message_When_Calling_Browse();

    void give_a_name();
};

} // namespace UPnPAV

#endif // MEDIASERVERSHOULD_H
