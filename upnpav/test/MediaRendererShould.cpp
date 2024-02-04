// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "MediaRendererShould.hpp"
#include "Descriptions.hpp"
#include "DeviceDescription.hpp"
#include "EventBackendDouble.hpp"
#include "InvalidDeviceDescription.hpp"
#include "MediaRenderer.hpp"
#include "SoapBackendDouble.hpp"
#include <QTest>
#include <utility>

namespace UPnPAV
{
namespace
{

class TestMediaRenderer : public MediaRenderer
{
    Q_OBJECT
public:
    TestMediaRenderer(DeviceDescription deviceDescription,
                      QSharedPointer<SoapBackendDouble> msgTransmitter,
                      QSharedPointer<Doubles::EventBackend> eventBackend)
        : MediaRenderer{deviceDescription, msgTransmitter, eventBackend}
        , mMsgTransmitter{std::move(msgTransmitter)}
        , mEventBackend{std::move(eventBackend)}
    {
    }

    QString lastSoapCall() const noexcept
    {
        return mMsgTransmitter->xmlMessageBody();
    }

private:
    QSharedPointer<SoapBackendDouble> mMsgTransmitter;
    QSharedPointer<Doubles::EventBackend> mEventBackend;
};

QSharedPointer<SoapBackendDouble> createSoapBackend()
{
    return QSharedPointer<SoapBackendDouble>::create();
}

QSharedPointer<Doubles::EventBackend> createEventBackend()
{
    return QSharedPointer<Doubles::EventBackend>::create();
}

TestMediaRenderer createMediaRenderer(QVector<ServiceDescription> const& services,
                                      QVector<ServiceControlPointDefinition> const& scpds)
{
    // clang-format off
    return TestMediaRenderer
    {
        DeviceDescription
        {
            QString(""),
            QStringLiteral("MediaRenderer"),
            QString(""),
            QString(""),
            QString(""),
            {IconDescription{"", 0, 0, 24, "http://localhost:8200/icons/sm.png"}},
            services,
            scpds
        },
        createSoapBackend(),
        createEventBackend()
    };
    // clang-format on
}

} // namespace

MediaRendererShould::~MediaRendererShould() = default;

void MediaRendererShould::throw_an_exception_when_the_device_description_has_no_rendering_control()
{
    auto desc = validRenderingControlServiceDescription();
    try {
        auto renderer = createMediaRenderer({validConnectionManagerDescription()}, {validConnectionManagerSCPD()});
        QFAIL("The constructor of the MediaRenderer should throw an exception");
    } catch (InvalidDeviceDescription& e) {
        QVERIFY(QString{e.what()}.contains("RenderingControl description not found"));
    }
}

void MediaRendererShould::throw_an_exception_when_the_rendering_control_description_is_not_correct_data()
{
    QTest::addColumn<DeviceDescription>("DeviceDescription");
    // clang-format off
    const auto rcNoUrl = DeviceDescription
    {
        QString(""), QString(""), QString(""), QString(""), QString(""),
        {},
        {validConnectionManagerDescription(), scpdUrlMissingInRenderingControlDescription()},
        {validConnectionManagerSCPD(), validRenderingControlSCPD()},
    };
    QTest::newRow("The RenderingControl service description has no URL") << rcNoUrl;

    const auto eventUrl = DeviceDescription
    {
        QString(""), QString(""), QString(""), QString(""), QString(""),
        {},
        {validConnectionManagerDescription(), eventUrlMissingInRenderingControlDescription()},
        {validConnectionManagerSCPD(), validRenderingControlSCPD()},
    };
    QTest::newRow("The RenderingControl service description has no event url") << eventUrl;

    const auto controlUrl = DeviceDescription
    {
        QString(""), QString(""), QString(""), QString(""), QString(""),
        {},
        {validConnectionManagerDescription(), controlUrlMissingInRenderingControlDescription()},
        {validConnectionManagerSCPD(), validRenderingControlSCPD()},
    };
    QTest::newRow("The RenderingControl service description has no control url") << controlUrl;
    
    const auto serviceUrl = DeviceDescription
    {
        QString(""), QString(""), QString(""), QString(""), QString(""),
        {},
        {validConnectionManagerDescription(), serviceUrlMissingInRenderingControlDescription()},
        {validConnectionManagerSCPD(), validRenderingControlSCPD()},
    };
    QTest::newRow("The RenderingControl service description has no service url") << serviceUrl;
    // clang-format on
}

void MediaRendererShould::throw_an_exception_when_the_rendering_control_description_is_not_correct()
{
    QFETCH(class DeviceDescription, DeviceDescription);
    QVERIFY_THROWS_EXCEPTION(InvalidDeviceDescription,
                             MediaRenderer{DeviceDescription, createSoapBackend(), createEventBackend()});
}

void MediaRendererShould::send_correct_soap_message_when_calling_get_volume()
{
    auto mediaRenderer =
        createMediaRenderer({validConnectionManagerDescription(), validRenderingControlServiceDescription()},
                            {validConnectionManagerSCPD(), validRenderingControlSCPD()});
    auto const expectedMessage = QString{"<?xml version=\"1.0\"?>"
                                         "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                                         "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
                                         "<s:Body>"
                                         "<u:GetVolume xmlns:u=\"urn:schemas-upnp-org:service:RenderingControl:1\">"
                                         "<InstanceID>2</InstanceID>"
                                         "<Channel>Master</Channel>"
                                         "</u:GetVolume>"
                                         "</s:Body>"
                                         "</s:Envelope>"};

    auto const call = mediaRenderer.volume(quint32{2}, QStringLiteral("Master"));

    QCOMPARE(call.has_value(), true);
    QCOMPARE(mediaRenderer.lastSoapCall(), expectedMessage);
}

void MediaRendererShould::send_correct_soap_message_when_calling_set_volume()
{
    auto mediaRenderer =
        createMediaRenderer({validConnectionManagerDescription(), validRenderingControlServiceDescription()},
                            {validConnectionManagerSCPD(), validRenderingControlSCPD()});
    auto const expectedMessage = QString{"<?xml version=\"1.0\"?>"
                                         "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                                         "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
                                         "<s:Body>"
                                         "<u:SetVolume xmlns:u=\"urn:schemas-upnp-org:service:RenderingControl:1\">"
                                         "<InstanceID>2</InstanceID>"
                                         "<Channel>Master</Channel>"
                                         "<DesiredVolume>98</DesiredVolume>"
                                         "</u:SetVolume>"
                                         "</s:Body>"
                                         "</s:Envelope>"};

    auto const call = mediaRenderer.setVolume(quint32{2}, QStringLiteral("Master"), quint32{98});

    QCOMPARE(call.has_value(), true);
    QCOMPARE(mediaRenderer.lastSoapCall(), expectedMessage);
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::MediaRendererShould);

#include "MediaRendererShould.moc"
