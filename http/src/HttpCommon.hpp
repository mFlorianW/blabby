// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <QByteArray>
#include <QHash>

namespace Http
{
/**
 * Type alias for HTTP hash table
 */
using Headers = QHash<QByteArray, QByteArray>;

} // namespace Http
