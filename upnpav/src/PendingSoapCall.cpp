// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "PendingSoapCall.hpp"
#include "SoapCall.hpp"
#include "private/ErrorResult.hpp"

#include <QVariant>

namespace UPnPAV
{
PendingSoapCall::PendingSoapCall(QSharedPointer<SoapCall> const& soapCall)
    : QObject()
    , m_soapCall(soapCall)
{
    Q_ASSERT(soapCall != nullptr);

    (void)connect(soapCall.get(), &SoapCall::finished, this, &PendingSoapCall::onSoapCallFinished);
}

PendingSoapCall::~PendingSoapCall() noexcept = default;

PendingSoapCall::ErrorCode PendingSoapCall::errorCode() const noexcept
{
    return m_errorCode;
}

QString const& PendingSoapCall::errorDescription() const noexcept
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

    if (!m_soapCall->hasFinishedSuccesful()) {
        ErrorResult errorResult{m_rawMessage};
        m_errorState = true;
        m_errorCode = convertErrorCode(errorResult.errorCode());
        m_errorDescription = errorResult.errorDescription();
    } else {
        m_errorCode = ErrorCode::NoError;
    }

    Q_EMIT finished();
}

PendingSoapCall::ErrorCode PendingSoapCall::convertErrorCode(qint32 errorCode)
{
    switch (errorCode) {
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
    case ErrorCode::InvalidConnectionReferenceOrWriteError:
        return ErrorCode::InvalidConnectionReferenceOrWriteError;
    case ErrorCode::IllegalMimeType:
        return ErrorCode::IllegalMimeType;
    case ErrorCode::ContentBusy:
        return ErrorCode::ContentBusy;
    case ErrorCode::ResourceNotFound:
        return ErrorCode::ResourceNotFound;
    case ErrorCode::InvalidInstanceId:
        return ErrorCode::InvalidInstanceId;
    case ErrorCode::NoDNSServer:
        return ErrorCode::NoDNSServer;
    case ErrorCode::BadDomainName:
        return ErrorCode::BadDomainName;
    case ErrorCode::ServerError:
        return ErrorCode::ServerError;
    case ErrorCode::TransitionNotAvailable:
        return ErrorCode::TransitionNotAvailable;
    case ErrorCode::TransportIsLocked:
        return ErrorCode::TransportIsLocked;
    case ErrorCode::NoContents:
        return ErrorCode::NoContents;
    case ErrorCode::ReadError:
        return ErrorCode::ReadError;
    case ErrorCode::FormatNotSupportedForPlayback:
        return ErrorCode::FormatNotSupportedForPlayback;
    case ErrorCode::PlaySpeedNotSupported:
        return ErrorCode::PlaySpeedNotSupported;
    case ErrorCode::MediaIsProtectedorNotWritable:
        return ErrorCode::MediaIsProtectedorNotWritable;
    case ErrorCode::FormatNotSupportedForRecording:
        return ErrorCode::FormatNotSupportedForRecording;
    case ErrorCode::MediaIsFull:
        return ErrorCode::MediaIsFull;
    case ErrorCode::SeekModeNotSupported:
        return ErrorCode::SeekModeNotSupported;
    case ErrorCode::IllegalSeekTarget:
        return ErrorCode::IllegalSeekTarget;
    case ErrorCode::RecordQualityNotSupported:
        return ErrorCode::RecordQualityNotSupported;
    case ErrorCode::PlayModeNotSupported:
        return ErrorCode::PlayModeNotSupported;
    default:
        return ErrorCode::Unknown;
    }
}

} // namespace UPnPAV
