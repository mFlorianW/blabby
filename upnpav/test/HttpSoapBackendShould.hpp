// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <QObject>

namespace UPnPAV
{

class HttpSoapBackendShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~HttpSoapBackendShould() override;
    Q_DISABLE_COPY_MOVE(HttpSoapBackendShould)

private Q_SLOTS:
    /**
     * @test
     * Tests that a SOAP call to a device that doesn't answer tells that the device is unreachable.
     */
    void tell_that_the_device_is_unreachable_when_nobody_answers();

    /**
     * @test
     * Tests that a SOAP call to a device that answers with a UPnP error doesn't tell that the device is unreachable.
     */
    void not_tell_that_the_device_is_unreachable_when_it_answers_with_an_error();
};

} // namespace UPnPAV
