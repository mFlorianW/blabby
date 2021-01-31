// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

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

    void give_a_icon_url();
};

} // namespace UPnPAV

#endif // MEDIASERVERSHOULD_H
