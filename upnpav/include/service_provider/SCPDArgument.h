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
#ifndef SCPDARGUMENT_H
#define SCPDARGUMENT_H

#include <QString>

namespace UPnPAV
{

class SCPDArgument final
{
public:
    enum Direction : quint8
    {
        In,
        Out
    };

    SCPDArgument();
    SCPDArgument(const QString &name,
                 Direction direction,
                 const QString &relatedStateVariable);

    QString name() const;

    Direction direction() const;

    QString relatedStateVariable() const;

    friend bool operator==(const SCPDArgument &lhs, const SCPDArgument &rhs);
    friend bool operator!=(const SCPDArgument &lhs, const SCPDArgument &rhs);

private:
    QString m_name;
    Direction m_direction;
    QString m_relatedStateVariable;
};

} //namespace UPnPAV

#endif // SCPDARGUMENT_H
