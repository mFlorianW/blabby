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
#ifndef PARSINGERROR_H
#define PARSINGERROR_H

#include <QException>

class ParsingError : public QException
{
public:
    /**
     *
     * @param errorDescription
     */
    ParsingError(const QString &errorDescription);

    /**
     *
     * @return
     */
    const char *what() const noexcept override;

    /**
     *
     */
    void raise() const override;

    /**
     *
     * @return
     */
    ParsingError *clone() const override;
private:
    QString m_errorDescription;
};

#endif // PARSINGERROR_H
