// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef UPNPAV_MEDIADEVICEBASETEST_H
#define UPNPAV_MEDIADEVICEBASETEST_H

#include <QObject>
#include <memory>

namespace UPnPAV
{
class ServiceControlPointDefinition;
class SCPDStateVariable;
class SCPDAction;
class IMediaDevice;
class DeviceDescription;

class MediaDeviceBaseTest : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(MediaDeviceBaseTest)
public:
    MediaDeviceBaseTest();
    ~MediaDeviceBaseTest() override;

protected:
    virtual std::unique_ptr<IMediaDevice> mediaDevice(const DeviceDescription &deviceDesc) = 0;

private:
    ServiceControlPointDefinition createConnectionManagerSCPDWithoutStateVariable(const SCPDStateVariable &variable);
    ServiceControlPointDefinition createConnectionManagerSCPDWithoutAction(const SCPDAction &action);

private Q_SLOTS:
    /**
     * @test The media device shall throw an exception when the ConnectionManager Service description
     * is missing.
     */
    void throw_An_Exception_When_DeviceDescription_Has_No_ConnectionManagerDescription();

    /**
     * @test The media device shall throw an exception when the ConnectionManager service
     * has no event URL set.
     */
    void throw_An_Exception_When_ConnectionManager_Description_Has_No_Event_Url();

    /**
     * @test The media device shall throw an exception when the ConnectionMangager service
     * has no control URL set.
     */
    void throw_An_Exception_When_ConnectionManager_Description_Has_No_Control_Url();

    /**
     * @test The media device shall throw an exception when the ConnectionManager service
     * has no service ID set.
     */
    void throw_An_Exception_When_ConnectionManager_Description_Has_No_ServiceId();

    /**
     * @test The media device shall throw an exception when the ConnectionManager service has no
     * SCPD url set.
     */
    void throw_An_Exception_When_ConnectionManager_Description_Has_No_SCPD_Url();

    /**
     * @test The media device shall throw an exception when the DeviceDescription
     * has no SCPD for the ConnectionManager.
     */
    void throw_An_Exception_When_DeviceDescription_Has_No_SCPD_For_ConnectionManager();

    void throw_Exception_When_StateVariable_Misses_In_ConnectionManager_SCPD_data();
    /**
     * @test The media device shall throw an exception on construction when the ConnectionManager SCPD misses one
     * of the minimum required state variables in the device description.
     */
    void throw_Exception_When_StateVariable_Misses_In_ConnectionManager_SCPD();

    void Throw_Exception_When_Action_Misses_in_ConnectionManager_SCPD_data();
    /**
     * @test The media device shall throw an exception on constructuion when the ConnectionManager SCPD misses one
     * of the minimum required actions in the device description.
     */
    void Throw_Exception_When_Action_Misses_in_ConnectionManager_SCPD();
};

} // namespace UPnPAV

#endif // UPNPAV_MEDIADEVICEBASETEST_H
