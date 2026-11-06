// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <QObject>
#include <QtQmlIntegration/qqmlintegration.h>

namespace Shell::SettingsRowKind
{
Q_NAMESPACE
QML_ELEMENT

/**
 * The kind of a row on the Settings screen, it decides which control shows the row.
 * QML reads the values as SettingsRowKind.Info, SettingsRowKind.Switch and SettingsRowKind.Action.
 */
enum Kind
{
    Info, ///< A row that shows a value, e.g. a version.
    Switch, ///< A row with a switch that turns a setting on or off.
    Action, ///< A row with a button whose tap is passed to a Provider.
};
Q_ENUM_NS(Kind)

} // namespace Shell::SettingsRowKind
