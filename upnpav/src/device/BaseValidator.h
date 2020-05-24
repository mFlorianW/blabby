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
