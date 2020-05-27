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
#ifndef SOAPMESSAGEGENERATOR_H
#define SOAPMESSAGEGENERATOR_H

#include <QString>
#include <QMap>

namespace UPnPAV
{
class SCPDAction;
struct Argument
{
    QString name;
    QString value;
};

using ArgumentList = QVector<Argument>;

class SoapMessageGenerator
{
public:
    SoapMessageGenerator();

    QString generateXmlMessageBody(const SCPDAction &action,
                                   const QString &serviceType,
                                   ArgumentList args = {}) noexcept;
private:
    static QVector<Argument> putArgumentsInOrderAsInDefiniton(const QVector<Argument> &args, const SCPDAction &action);
};

} //namespace UPnPAV

#endif // SOAPMESSAGEGENERATOR_H
