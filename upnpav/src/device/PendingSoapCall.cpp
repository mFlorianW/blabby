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
#include "PendingSoapCall.h"
#include "SoapCall.h"
#include "ErrorResult.h"

#include <QVariant>

namespace UPnPAV
{
PendingSoapCall::PendingSoapCall(const QSharedPointer<SoapCall> &soapCall)
    : QObject()
    , m_soapCall(soapCall)
{
    Q_ASSERT(soapCall != nullptr);

    (void)connect(soapCall.get(),
                  &SoapCall::finished,
                  this,
                  &PendingSoapCall::onSoapCallFinished);
}

PendingSoapCall::ErrorCode PendingSoapCall::errorCode() const noexcept
{
    return m_errorCode;
}

const QString &PendingSoapCall::errorDescription() const noexcept
{
    return m_errorDescription;
}

bool PendingSoapCall::hasError() const noexcept
{
    return m_errorState;
}

void PendingSoapCall::onSoapCallFinished()
{
    m_rawMessage = m_soapCall->rawMessage();

    if(!m_soapCall->hasFinishedSuccesful())
    {
        ErrorResult errorResult{m_rawMessage};
        m_errorState = true;
        m_errorCode = convertErrorCode(errorResult.errorCode());
        m_errorDescription = errorResult.errorDescription();
    }
    else
    {
        m_errorCode = ErrorCode::NoError;
    }

    Q_EMIT finished();
}

PendingSoapCall::ErrorCode PendingSoapCall::convertErrorCode(qint32 errorCode)
{
    switch (errorCode)
    {
        case ErrorCode::NoError:
            return ErrorCode::NoError;
        case ErrorCode::InvalidAction:
            return ErrorCode::InvalidAction;
        case ErrorCode::InvalidArgs:
            return ErrorCode::InvalidArgs;
        case ErrorCode::ActionFailed:
            return ErrorCode::ActionFailed;
        case ErrorCode::ArgumentValueInvalid:
            return ErrorCode::ArgumentValueInvalid;
        case ErrorCode::ArgumentValueOutOfRange:
            return ErrorCode::ArgumentValueOutOfRange;
        case ErrorCode::OptionalActionNotImplemented:
            return ErrorCode::OptionalActionNotImplemented;
        case ErrorCode::OutOfMemory:
            return ErrorCode::OutOfMemory;
        case ErrorCode::HumanInterventionRequired:
            return ErrorCode::HumanInterventionRequired;
        case ErrorCode::StringArgumentTooLong:
            return ErrorCode::StringArgumentTooLong;
        case ErrorCode::ActionNotAuthorized:
            return ErrorCode::ActionNotAuthorized;
        case ErrorCode::SignatureFailure:
            return ErrorCode::SignatureFailure;
        case ErrorCode::SignatureMissing:
            return ErrorCode::SignatureMissing;
        case ErrorCode::NotEncrypted:
            return ErrorCode::NotEncrypted;
        case ErrorCode::InvalidSequence:
            return ErrorCode::InvalidSequence;
        case ErrorCode::InvalidControlUrl:
            return ErrorCode::InvalidControlUrl;
        case ErrorCode::NoSuchSession:
            return ErrorCode::NoSuchSession;
        default:
            return ErrorCode::Unknown;
    }
}

} //namespace UPnPAV
