// Copyright 2019 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef PENDINGSOAPCALL_H
#define PENDINGSOAPCALL_H

#include "blabbyupnpav_export.h"
#include <QSharedPointer>
#include <QString>

namespace UPnPAV
{
class SoapCall;

class BLABBYUPNPAV_EXPORT PendingSoapCall : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(PendingSoapCall)
public:
    enum ErrorCode
    {
        ///< SoapCall finished with an unknown error or is not finished.
        Unknown = 0,
        ///< SoapCall finished successful.
        NoError = 200,
        ///< No action by that name at this service.
        InvalidAction = 401,
        ///< Not enough in args, too many in args, no in arg by that name, one or more in args are of the wrong data
        ///  type.
        InvalidArgs = 402,
        ///< May be returned in current state of service prevents invoking that action.
        ActionFailed = 501,
        ///< The argument value is invalid
        ArgumentValueInvalid = 600,
        ///< An argument value is less than the minimum or more than the maximum value of the allowedValueRange, or is
        ///  not in the allowedValueList.
        ArgumentValueOutOfRange = 601,
        ///< The requested action is optional and is not implemented by the device.
        OptionalActionNotImplemented = 602,
        ///< The device does not have sufficient memory available to complete the action.
        ///  This may be a temporary condition; the control point may choose to retry the
        /// unmodified request again later and it may succeed if memory is available.
        OutOfMemory = 603,
        ///< The device has encountered an error condition which it cannot resolve itself and required human
        ///  intervention such as a reset or power cycle. See the device display or documentation for further guidance.
        HumanInterventionRequired = 604,
        ///< A string argument is too long for the device to handle properly.
        StringArgumentTooLong = 605,
        ///< The action requested requires authorization and the sender was not authorized.
        ActionNotAuthorized = 606,
        ///< The sender's signature failed to verify.
        SignatureFailure = 607,
        ///< The action requested requires a digital signature and there was none provided.
        SignatureMissing = 608,
        /// This action requires confidentiality but the action was not delivered encrypted.
        NotEncrypted = 609,
        ///< The <sequence> provided was not valid.
        InvalidSequence = 610,
        ///< The controlURL within the <freshness> element does not match the controlURL of  the action actually invoked
        ///  (or the controlURL in the HTTP header).
        InvalidControlUrl = 611,
        ///< The session key reference is to a non-existent session. This could be because the device has expired a
        /// session, in which case the control point needs to open a new one.
        NoSuchSession = 612
    };
    Q_ENUM(ErrorCode)

    /**
     * Coonsturcts a PendingSoapCall.
     *
     * @param soapCall The explicit soap call.
     */
    PendingSoapCall(const QSharedPointer<SoapCall> &soapCall);

    /**
     * Gives the return of the pending soap call. Is only valid after the finished signal
     * is emitted.
     *
     * @return The error code of the SOAP call only valid after the finish signal otherwise
     *         the function will return unknown.
     */
    ErrorCode errorCode() const noexcept;

    /**
     * Gives the a detailed description about the error. The description is only valid when
     * finish signal is emitted. It's not required that the description is set by the server
     * so maybe it's possible that the description is empty.
     *
     * @return The error description for the case when the call failed.
     */
    const QString &errorDescription() const noexcept;

    /**
     * Indicates the result of the SOAP call. True means the call finished without
     * success. The result is only valid when the finished signal was emitted
     * without the signal the function will return false.
     *
     * @return True the call finished with error.
     */
    bool hasError() const noexcept;

    /**
     * Creates an object of type T and passes the raw message to the contructor. The contructor
     * of the object must accept a const QString& as first parameter. The function can be used
     * to directly instaniate result object of the SOAP call.
     *
     * @The function may throw an exception when the constructor of T throws an exception
     * durinng construction.
     *
     * @return Returns an instance of T.
     */
    template <typename T>
    QSharedPointer<T> resultAs() const;

Q_SIGNALS:
    /**
     * This signal is emitted when the SOAP is call is finished.
     */
    void finished();

private Q_SLOTS:
    /**
     * Handles the return values of the SOAP call.
     */
    void onSoapCallFinished();

private:
    static ErrorCode convertErrorCode(qint32 errorCode);

private:
    QSharedPointer<SoapCall> m_soapCall;
    bool m_errorState{false};
    ErrorCode m_errorCode{Unknown};
    QString m_errorDescription;
    QString m_rawMessage;
};

template <typename T>
QSharedPointer<T> PendingSoapCall::resultAs() const
{
    return QSharedPointer<T>{new T(m_rawMessage)};
}

} // namespace UPnPAV

#endif // PENDINGSOAPCALL_H
