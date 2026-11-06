// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "Item.hpp"
#include "Queue.hpp"
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
 * Makes @ref Multimedia::Queue known to QML as Queue, e.g. for Queue.All.
 */
struct QueueForeign
{
    Q_GADGET
    QML_FOREIGN(Multimedia::Queue)
    QML_NAMED_ELEMENT(Queue)
    QML_UNCREATABLE("The Queue is provided by the QueueModel")
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
