// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "ServerResponse.hpp"
#include "blabbyhttp_export.h"
#include <QObject>

namespace Http
{

/**
 * The @ref Htpp::ResponseBuilder builds the byte stream from a @ref Http::ServerResponse
 */
class BLABBYHTTP_EXPORT ResponseSerializer final : public QObject
{
    Q_OBJECT
public:
    /**
     * Creates an instance of the @ref Http::ResponseSerializer
     * @param serverResponse The @ref Http::ServerResponse that shall be serialized.
     */
    ResponseSerializer(ServerResponse serverResponse);

    /**
     * Default destructor
     */
    ~ResponseSerializer() override;

    /*
     * Disabled copy and move semantic
     */
    Q_DISABLE_COPY_MOVE(ResponseSerializer)

public Q_SLOTS:
    /**
     * Serialize the server response
     * @note
     * The signal @ref Http::ResponseSerializer::responseSerialized is emitted when finished.
     */
    void serialize() noexcept;

Q_SIGNALS:
    /**
     * This signal is emitted when the @ref Http::ServerResponse is serialized.
     * @param response The serialized response.
     */
    void responseSerialized(QByteArray const& response);

private:
    ServerResponse mResponse;
};

} // namespace Http
