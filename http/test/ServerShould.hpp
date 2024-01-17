// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

// #include "Server.hpp"
#include <QObject>

namespace Http
{
class ServerShould : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
    ~ServerShould() override;
    Q_DISABLE_COPY_MOVE(ServerShould)

private Q_SLOTS:

private:
    // std::unique_ptr<Server> mServer;
};

} // namespace Http
