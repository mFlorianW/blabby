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
#ifndef SOAPCALL_H
#define SOAPCALL_H

#include <QObject>

namespace UPnPAV
{

class SoapCall : public QObject
{
    Q_OBJECT
public:
    virtual ~SoapCall();

    /**
     * Status of the SOAP call. True means call finished successful otherwise false.
     *
     * @note The return value is only valid when finished signal was emitted.
     *
     * @return True call finished successful, otherwise false.
     */
    virtual bool hasFinishedSuccesful() const noexcept = 0;

    /**
     * Raw answer of the error code.
     *
     * @return The raw response of the Soap call.
     */
    virtual QString rawMessage() const noexcept = 0;

Q_SIGNALS:
    /**
     * This signal shall be emitted when the Soap call
     * is finished.
     */
    void finished();
};

} //namespace UPnPAV

#endif // SOAPCALL_H
