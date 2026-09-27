// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "Renderer.hpp"
#include <QObject>
#include <QQmlEngine>

namespace Shell
{

/**
 * Registers the C++ types that the QML under test needs, like main does for the shell.
 */
class QmlTestsSetup : public QObject
{
    Q_OBJECT

public Q_SLOTS:
    void qmlEngineAvailable(QQmlEngine* engine)
    {
        Q_UNUSED(engine)
        qmlRegisterUncreatableType<Multimedia::Renderer>("Blabby.Objects", 1, 0, "Renderer", "");
    }
};

} // namespace Shell
