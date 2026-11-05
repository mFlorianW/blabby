// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021-2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "ActiveRendererController.hpp"
#include "MediaItemModel.hpp"
#include "MediaRendererModel.hpp"
#include "MediaSourceModel.hpp"
#include "QueueModel.hpp"
#include "SettingsModel.hpp"
#include <QObject>
#include <QQmlEngine>
#include <qqmlengine.h>

namespace Shell
{

/**
 * This class is the main registry for every singleton that is used in the QML context.
 * All the signleton types can be accesed by its property.
 */
class QmlSingletonRegistry : public QObject
{
    Q_OBJECT
    /**
     * Gives the MediaSourceModel
     */
    Q_PROPERTY(Shell::MediaSourceModel* mediaSourceModel READ mediaSourceModel CONSTANT)

    /**
     * Gives the MediaItemModel
     */
    Q_PROPERTY(Shell::MediaItemModel* mediaItemModel READ mediaItemModel CONSTANT)

    /**
     * Gives the MediaRendererModel
     */
    Q_PROPERTY(Shell::MediaRendererModel* mediaRendererModel READ mediaRendererModel CONSTANT)

    /**
     * Gives the ActiveRendererController
     */
    Q_PROPERTY(Shell::ActiveRendererController* activeRendererController READ activeRendererController CONSTANT)

    /**
     * Gives the QueueModel
     */
    Q_PROPERTY(Shell::QueueModel* queueModel READ queueModel CONSTANT)

    /**
     * Gives the SettingsModel
     */
    Q_PROPERTY(Shell::SettingsModel* settingsModel READ settingsModel CONSTANT)

public:
    /**
     * Default destructor
     */
    ~QmlSingletonRegistry() override;

    /**
     * Disabled copy and move semantic.
     */
    Q_DISABLE_COPY_MOVE(QmlSingletonRegistry)

    /**
     * Gives the Providers the shell loaded once at startup, every part of the shell works with these.
     * @return The loaded Providers that initialized.
     */
    Multimedia::Providers const& providers() const noexcept;

    /**
     * Gives a pointer to the QMLEngine for the @ref Shell::MediaSourceModel singleton.
     * The pointer has the same lifetime as the @ref Shell::QmlSingletonRegistry instance.
     * @note The callee doesn't take the ownership of the returned pointer.
     * @return A pointer to the MediaSourceModel instance
     */
    MediaSourceModel* mediaSourceModel() noexcept;

    /**
     * Gives a pointer to the QMLEngine for the @ref Shell::MediaItemModel singleton.
     * The pointer has the same lifetime as the @ref Shell::QmlSingletonRegistry instance.
     * @note The callee doesn't take the ownership of the returned pointer.
     * @return A pointer to the @ref Shell::MediaItemModel instance
     */
    MediaItemModel* mediaItemModel() noexcept;

    /**
     * Gives a pointer to the QMLEngine for the @ref Shell::MediaRendererModel singleton.
     * The pointer has the same lifetime as the @ref Shell::QmlSingletonRegistry instance.
     * @note The callee doesn't take the ownership of the returned pointer.
     * @return A pointer to the @ref Shell::MediaItemModel instance
     */
    MediaRendererModel* mediaRendererModel() noexcept;

    /**
     * Gives a pointer to the QMLEngine for the @ref Shell::ActiveRendererController singleton.
     * The pointer has the same lifetime as the @ref Shell::QmlSingletonRegistry instance.
     * @note The callee doesn't take the ownership of the returned pointer.
     * @return A pointer to the @ref Shell::ActiveRendererController instance
     */
    ActiveRendererController* activeRendererController() noexcept;

    /**
     * Gives a pointer to the QMLEngine for the @ref Shell::QueueModel singleton.
     * The pointer has the same lifetime as the @ref Shell::QmlSingletonRegistry instance.
     * @note The callee doesn't take the ownership of the returned pointer.
     * @return A pointer to the @ref Shell::QueueModel instance
     */
    QueueModel* queueModel() noexcept;

    /**
     * Gives a pointer to the QMLEngine for the @ref Shell::SettingsModel singleton.
     * The pointer has the same lifetime as the @ref Shell::QmlSingletonRegistry instance.
     * @note The callee doesn't take the ownership of the returned pointer.
     * @return A pointer to the @ref Shell::SettingsModel instance
     */
    SettingsModel* settingsModel() noexcept;

    /**
     * Callback for the QML engine for creating an instance of the @ref Shell::QmlSingletonRegistry
     * @param engine Unused
     * @param scriptEngine Unused
     * @return QObject pointer to the QmlSingletonRegistry instance.
     *
     * @note The function should only be called by the QML engine.
     */
    static QObject* createQmlRegistry(QQmlEngine* engine, QJSEngine* scriptEngine);

protected:
    /**
     * Cerates an instance of the QmlSingletonRegistry.
     * All Singleton are instanciated in the contrusctor.
     */
    QmlSingletonRegistry();

private:
    Multimedia::Providers mProviders;
    MediaSourceModel mSourceModel;
    MediaItemModel mItemModel;
    MediaRendererModel mRendererModel;
    ActiveRendererController mActiveRendererController;
    QueueModel mQueueModel;
    SettingsModel mSettingsModel;
};

} // namespace Shell
