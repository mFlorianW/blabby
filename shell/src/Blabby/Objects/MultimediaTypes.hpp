// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "Item.hpp"
#include "Renderer.hpp"
#include <QObject>
#include <QtQml/qqml.h>

/**
 * Makes the QML types of the Multimedia library known to QML in the module Blabby.Objects.
 * The Multimedia library doesn't depend on QML, so its types are declared here as foreign types.
 */
namespace Shell::Objects
{

/**
 * Makes @ref Multimedia::Renderer known to QML as Renderer, e.g. for Renderer.Online.
 */
struct RendererForeign
{
    Q_GADGET
    QML_FOREIGN(Multimedia::Renderer)
    QML_NAMED_ELEMENT(Renderer)
    QML_UNCREATABLE("Renderers are provided by the MediaRendererModel")
};

/**
 * Makes the enums of the namespace Multimedia known to QML as ItemType, e.g. for ItemType.Container.
 */
namespace ItemTypeForeign
{
Q_NAMESPACE
QML_FOREIGN_NAMESPACE(Multimedia)
QML_NAMED_ELEMENT(ItemType)
} // namespace ItemTypeForeign

} // namespace Shell::Objects
