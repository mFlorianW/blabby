// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef BASEVALIDATOR_H
#define BASEVALIDATOR_H

#include "blabbyupnpav_export.h"
#include <QString>

namespace UPnPAV
{

/**
 * Base class for all kind of validators.
 */
class BLABBYUPNPAV_EXPORT BaseValidator
{
public:
    /**
     * Creates an instance of the BaseValidator
     */
    BaseValidator() = default;

    /**
     * Default destructor
     */
    virtual ~BaseValidator();

    /**
     * Default copy constructor
     */
    BaseValidator(BaseValidator const &other) = default;

    /**
     * Default move constructor
     */
    BaseValidator(BaseValidator &&other) noexcept = default;

    /**
     * Default copy assignment
     */
    BaseValidator &operator=(BaseValidator const &ohter) = default;

    /**
     * Default move assignment
     */
    BaseValidator &operator=(BaseValidator &&ohter) noexcept = default;

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
    QString const &errorMessage() const noexcept;

protected:
    QString m_errorMessage;
};

} // namespace UPnPAV

#endif // BASEVALIDATOR_H
