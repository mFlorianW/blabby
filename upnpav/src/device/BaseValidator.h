// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef BASEVALIDATOR_H
#define BASEVALIDATOR_H

#include <QString>

namespace UPnPAV
{

/**
 * Base class for all kind of validators.
 */
class BaseValidator
{
public:
    virtual ~BaseValidator();

    /**
     * The validation implementaion.
     *
     * @return True validation success otherwise false.
     */
    virtual bool validate() noexcept = 0;

    /**
     * Gives the error message when the validation wasn't successful. The string will
     * be empty if validate returns true.
     *
     * @return The error message or empty string if validate return true.
     */
    const QString &errorMessage() const noexcept;

protected:
    QString m_errorMessage;
};

} //namespace UPnPAv

#endif // BASEVALIDATOR_H
