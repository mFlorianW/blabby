// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later
#include "MediaRendererShould.hpp"
#include "Descriptions.hpp"
#include "DeviceDescription.hpp"
#include "IconDescription.hpp"
#include "InvalidDeviceDescription.hpp"
#include "MediaRenderer.hpp"
#include "SoapMessageTransmitterDouble.hpp"
#include <QTest>

namespace UPnPAV
{
namespace
{

QSharedPointer<SoapMessageTransmitterDouble> createSoapMessageTransmitter()
{
    return QSharedPointer<SoapMessageTransmitterDouble>::create();
}

MediaRenderer createMediaRenderer(QVector<ServiceDescription> const &services,
                                  QVector<ServiceControlPointDefinition> const &scpds)
{
    // clang-format off
    return MediaRenderer
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
        createSoapMessageTransmitter()
    };
    // clang-format on
}

} // namespace

MediaRendererShould::~MediaRendererShould() = default;

void MediaRendererShould::throw_an_exception_when_the_device_description_has_no_rendering_control()
{
    auto desc = validRenderingControlServiceDescription();
    try
    {
        auto renderer = createMediaRenderer({validConnectionManagerDescription()}, {validConnectionManagerSCPD()});
        QFAIL("The constructor of the MediaRenderer should throw an exception");
    }
    catch (InvalidDeviceDescription &e)
    {
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
                             MediaRenderer{DeviceDescription, createSoapMessageTransmitter()});
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::MediaRendererShould);
