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
#ifndef SOAPCALLDOUBLE_H
#define SOAPCALLDOUBLE_H

#include <SoapCall.h>

namespace UPnPAV
{

class SoapCallDouble : public SoapCall
{
public:
    SoapCallDouble();

    void setErrorState(bool error);
    bool hasFinishedSuccesful() const noexcept override;

    void setRawMessage(const QString &rawMessage);
    QString rawMessage() const noexcept override;

private:
    bool m_errorState;
    QString m_rawMessage;
};

} //namespace UPnPAV

#endif // SOAPCALLDOUBLE_H
