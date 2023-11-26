// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "MediaSourceModel.hpp"
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
    Q_PROPERTY(Shell::MediaSourceModel *mediaSourceModel READ mediaSourceModel CONSTANT)
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
     * Gives a pointer to the QMLEngine for the @ref Shell::MediaSourceModel singleton.
     * The pointer has the same lifetime as the @ref Shell::QmlSingletonRegistry instance.
     * @note The callee doesn't take the ownership of the returned pointer.
     * @return A pointer to the MediaSourceModel instance
     */
    MediaSourceModel *mediaSourceModel() noexcept;

    /**
     * Callback for the QML engine for creating an instance of the @ref Shell::QmlSingletonRegistry
     * @param engine Unused
     * @param scriptEngine Unused
     * @return QObject pointer to the QmlSingletonRegistry instance.
     *
     * @note The function should only be called by the QML engine.
     */
    static QObject *createQmlRegistry(QQmlEngine *engine, QJSEngine *scriptEngine);

protected:
    /**
     * Cerates an instance of the QmlSingletonRegistry.
     * All Singleton are instanciated in the contrusctor.
     */
    QmlSingletonRegistry();

private:
    MediaSourceModel mSourceModel;
};

} // namespace Shell
