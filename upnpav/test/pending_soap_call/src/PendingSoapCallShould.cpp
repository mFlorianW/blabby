// SPDX-FileCopyrightText: 2021 Florian Weßel <florianwessel@gmx.net>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "PendingSoapCallShould.h"
#include "PendingSoapCall.h"
#include "SoapCallDouble.h"

#include <QTest>
#include <QSignalSpy>

namespace
{

class TestObject
{
public:
    TestObject(const QString &rawMessage)
        : m_rawMessage(rawMessage)
    {
    }

    QString rawMessage() const noexcept
    {
        return m_rawMessage;
    }

private:
    QString m_rawMessage;
};

static constexpr char errorXML[] =
{
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
    "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
       "<s:Body>"
          "<s:Fault>"
             "<faultcode>s:Client</faultcode>"
             "<faultstring>UPnPError</faultstring>"
             "<detail>"
                "<UPnPError xmlns=\"urn:schemas-upnp-org:control-1-0\">"
                   "<errorCode>%1</errorCode>"
                   "<errorDescription>%2</errorDescription>"
                "</UPnPError>"
             "</detail>"
          "</s:Fault>"
       "</s:Body>"
    "</s:Envelope>"
};

} //namespace

namespace UPnPAV
{

PendingSoapCallShould::PendingSoapCallShould()
{
}

void PendingSoapCallShould::emit_Finished_Signal_When_DataReceived()
{
    QSharedPointer<SoapCallDouble> soapCallDouble
    {
        new SoapCallDouble()
    };

    PendingSoapCall pendingSoapCall
    {
        soapCallDouble
    };


    QSignalSpy signalSpy
    {
        &pendingSoapCall,
        &PendingSoapCall::finished
    };

    soapCallDouble->finished();

    QVERIFY2(signalSpy.count() == 1,
             QString{"Expected: %1 \n Actual: %2"}.arg("1").arg(signalSpy.count()).toLocal8Bit());
}

void PendingSoapCallShould::set_Error_To_True_When_Soap_Call_Finished_With_Error()
{
    QSharedPointer<SoapCallDouble> soapCallDouble
    {
        new SoapCallDouble()
    };
    soapCallDouble->setErrorState(true);

    PendingSoapCall pendingSoapCall
    {
        soapCallDouble
    };

    soapCallDouble->finished();
    auto hasError = pendingSoapCall.hasError();

    QVERIFY2(true == hasError,
             QString{"Expected %1 Actual %2"}.arg(true).arg(hasError).toLocal8Bit());
}

void PendingSoapCallShould::map_Soap_Error_Codes_To_Enum_When_Call_Finished_data()
{
    QTest::addColumn<QSharedPointer<SoapCallDouble>>("SoapCallWithError");
    QTest::addColumn<PendingSoapCall::ErrorCode>("ExpectedErrorCode");

    QSharedPointer<SoapCallDouble> soapCallDoubleNoError
    {
        new SoapCallDouble()
    };
    soapCallDoubleNoError->setErrorState(false);

    QTest::newRow("Error code should be NoError") << soapCallDoubleNoError
                                                  << PendingSoapCall::ErrorCode::NoError;

    qint32 invalidActionErroCode = 401;
    QString invalidActionXML = QString{errorXML}.arg(invalidActionErroCode);
    QSharedPointer<SoapCallDouble> soapCallDoubleInvalidAction{new SoapCallDouble()};
    soapCallDoubleInvalidAction->setErrorState(true);
    soapCallDoubleInvalidAction->setRawMessage(invalidActionXML);
    QTest::newRow("Should be InvalidAction") << soapCallDoubleInvalidAction
                                             << PendingSoapCall::ErrorCode::InvalidAction;

    qint32 invalidArgsErroCode = 402;
    QString invalidArgsErroCodeXML = QString{errorXML}.arg(invalidArgsErroCode);
    QSharedPointer<SoapCallDouble> soapCallDoubleInvalidArgs{new SoapCallDouble()};
    soapCallDoubleInvalidArgs->setErrorState(true);
    soapCallDoubleInvalidArgs->setRawMessage(invalidArgsErroCodeXML);
    QTest::newRow("Should be InvalidArgs") << soapCallDoubleInvalidArgs
                                           << PendingSoapCall::ErrorCode::InvalidArgs;

    qint32 actionFaileddErrorCode = 501;
    QString actionFaileddErrorCodeXML = QString{errorXML}.arg(actionFaileddErrorCode);
    QSharedPointer<SoapCallDouble> soapCallDoubleActionFailed{new SoapCallDouble()};
    soapCallDoubleActionFailed->setErrorState(true);
    soapCallDoubleActionFailed->setRawMessage(actionFaileddErrorCodeXML);
    QTest::newRow("Should be ActionFailed") << soapCallDoubleActionFailed
                                            << PendingSoapCall::ErrorCode::ActionFailed;

    qint32 argumentValueInvaliddErrorCode = 600;
    QString argumentValueInvaliddErrorCodeXML = QString{errorXML}.arg(argumentValueInvaliddErrorCode);
    QSharedPointer<SoapCallDouble> soapCallDoubleArgumentValueInvalid{new SoapCallDouble()};
    soapCallDoubleArgumentValueInvalid->setErrorState(true);
    soapCallDoubleArgumentValueInvalid->setRawMessage(argumentValueInvaliddErrorCodeXML);
    QTest::newRow("Should be Argument value invalid") << soapCallDoubleArgumentValueInvalid
                                                      << PendingSoapCall::ErrorCode::ArgumentValueInvalid;

    qint32 argumentValueOutOfRangeInvaliddErrorCode = 601;
    QString argumentValueOutOfRangeInvaliddErroCodeXML = QString{errorXML}.arg(argumentValueOutOfRangeInvaliddErrorCode);
    QSharedPointer<SoapCallDouble> soapCallDoubleArgumentValueOutOfrange{new SoapCallDouble()};
    soapCallDoubleArgumentValueOutOfrange->setErrorState(true);
    soapCallDoubleArgumentValueOutOfrange->setRawMessage(argumentValueOutOfRangeInvaliddErroCodeXML);
    QTest::newRow("Should be Argument value out of range") << soapCallDoubleArgumentValueOutOfrange
                                                           << PendingSoapCall::ErrorCode::ArgumentValueOutOfRange;

    qint32 optionalActionNotImplementeddErrorCode = 602;
    QString optionalActionNotImplementeddErrorCodeXML = QString{errorXML}.arg(optionalActionNotImplementeddErrorCode);
    QSharedPointer<SoapCallDouble> soapCallDoubleOptionalActionNotImplemented{new SoapCallDouble()};
    soapCallDoubleOptionalActionNotImplemented->setErrorState(true);
    soapCallDoubleOptionalActionNotImplemented->setRawMessage(optionalActionNotImplementeddErrorCodeXML);
    QTest::newRow("Should be optional action not implemented.") << soapCallDoubleOptionalActionNotImplemented
                                                               << PendingSoapCall::ErrorCode::OptionalActionNotImplemented;

    qint32 outOfMemoryErroCode = 603;
    QString outOfMemoryErrorCodeXML = QString{errorXML}.arg(outOfMemoryErroCode);
    QSharedPointer<SoapCallDouble> soapCallDoubleOutOfMemory{new SoapCallDouble()};
    soapCallDoubleOutOfMemory->setErrorState(true);
    soapCallDoubleOutOfMemory->setRawMessage(outOfMemoryErrorCodeXML);
    QTest::newRow("Should be out of memory") << soapCallDoubleOutOfMemory
                                             << PendingSoapCall::ErrorCode::OutOfMemory;

    qint32 humanInverventionRequiredErrorCode = 604;
    QString humanInverventionRequiredErrorCodeXML = QString{errorXML}.arg(humanInverventionRequiredErrorCode);
    QSharedPointer<SoapCallDouble> soapCallDoubleHumanInterventionRequired{new SoapCallDouble()};
    soapCallDoubleHumanInterventionRequired->setErrorState(true);
    soapCallDoubleHumanInterventionRequired->setRawMessage(humanInverventionRequiredErrorCodeXML);
    QTest::newRow("Should be human intervention required") << soapCallDoubleHumanInterventionRequired
                                                           << PendingSoapCall::ErrorCode::HumanInterventionRequired;

    qint32 stringArgumentTooLongErrorCode = 605;
    QString stringArgumentTooLongErrorCodeXML = QString{errorXML}.arg(stringArgumentTooLongErrorCode);
    QSharedPointer<SoapCallDouble> soapCallDoubleStringArgumentTooLong{new SoapCallDouble()};
    soapCallDoubleStringArgumentTooLong->setErrorState(true);
    soapCallDoubleStringArgumentTooLong->setRawMessage(stringArgumentTooLongErrorCodeXML);
    QTest::newRow("Should be string argument to long") << soapCallDoubleStringArgumentTooLong
                                                       << PendingSoapCall::ErrorCode::StringArgumentTooLong;

    qint32 actionNotAuhorizedErrorCode = 606;
    QString actionNotAuhorizedErrorCodeXML = QString{errorXML}.arg(actionNotAuhorizedErrorCode);
    QSharedPointer<SoapCallDouble> soapCallDoubleActionNotAuthorized{new SoapCallDouble()};
    soapCallDoubleActionNotAuthorized->setErrorState(true);
    soapCallDoubleActionNotAuthorized->setRawMessage(actionNotAuhorizedErrorCodeXML);
    QTest::newRow("Should be action not authorized") << soapCallDoubleActionNotAuthorized
                                                     << PendingSoapCall::ErrorCode::ActionNotAuthorized;

    qint32 signatureFailingErrorCode = 607;
    QString signatureFailingErrorCodeXML = QString{errorXML}.arg(signatureFailingErrorCode);
    QSharedPointer<SoapCallDouble> soapCallDoubleSignatureFailure{new SoapCallDouble()};
    soapCallDoubleSignatureFailure->setErrorState(true);
    soapCallDoubleSignatureFailure->setRawMessage(signatureFailingErrorCodeXML);
    QTest::newRow("Should be signature failure") << soapCallDoubleSignatureFailure
                                                 << PendingSoapCall::ErrorCode::SignatureFailure;

    qint32 signatureMissingErrorCode = 608;
    QString signatureMissingErrorCodeXML = QString{errorXML}.arg(signatureMissingErrorCode);
    QSharedPointer<SoapCallDouble> soapCallDoubleSignatureMissing{new SoapCallDouble()};
    soapCallDoubleSignatureMissing->setErrorState(true);
    soapCallDoubleSignatureMissing->setRawMessage(signatureMissingErrorCodeXML);
    QTest::newRow("Should be signature missing") << soapCallDoubleSignatureMissing
                                                 << PendingSoapCall::ErrorCode::SignatureMissing;

    qint32 notEncryptedErrorCode = 609;
    QString notEncryptedErrorCodeXML = QString{errorXML}.arg(notEncryptedErrorCode);
    QSharedPointer<SoapCallDouble> soapCallDoubleNotEncrypted{new SoapCallDouble()};
    soapCallDoubleNotEncrypted->setErrorState(true);
    soapCallDoubleNotEncrypted->setRawMessage(notEncryptedErrorCodeXML);
    QTest::newRow("Should be not encrypted") << soapCallDoubleNotEncrypted
                                             << PendingSoapCall::ErrorCode::NotEncrypted;

    qint32 invalidSequenceErrorCode = 610;
    QString invalidSequenceErrorCodeXML = QString{errorXML}.arg(invalidSequenceErrorCode);
    QSharedPointer<SoapCallDouble> soapCallDoubleInvalidSequence{new SoapCallDouble()};
    soapCallDoubleInvalidSequence->setErrorState(true);
    soapCallDoubleInvalidSequence->setRawMessage(invalidSequenceErrorCodeXML);
    QTest::newRow("Should be invalid sequence") << soapCallDoubleInvalidSequence
                                             << PendingSoapCall::ErrorCode::InvalidSequence;

    qint32 invalidControlUrlErrorCode = 611;
    QString invalidControlUrlErrorCodeXML = QString{errorXML}.arg(invalidControlUrlErrorCode);
    QSharedPointer<SoapCallDouble> soapCallDoubleInvalidControlUrl{new SoapCallDouble()};
    soapCallDoubleInvalidControlUrl->setErrorState(true);
    soapCallDoubleInvalidControlUrl->setRawMessage(invalidControlUrlErrorCodeXML);
    QTest::newRow("Should be invalid control url") << soapCallDoubleInvalidControlUrl
                                                   << PendingSoapCall::ErrorCode::InvalidControlUrl;

    qint32 noSuchSessionUrlErrorCode = 612;
    QString noSuchSessionUrlErrorCodeXML = QString{errorXML}.arg(noSuchSessionUrlErrorCode);
    QSharedPointer<SoapCallDouble> soapCallDoubleNoSuchSession{new SoapCallDouble()};
    soapCallDoubleNoSuchSession->setErrorState(true);
    soapCallDoubleNoSuchSession->setRawMessage(noSuchSessionUrlErrorCodeXML);
    QTest::newRow("Should be nu such session") << soapCallDoubleNoSuchSession
                                               << PendingSoapCall::ErrorCode::NoSuchSession;
}

void PendingSoapCallShould::map_Soap_Error_Codes_To_Enum_When_Call_Finished()
{
    QFETCH(QSharedPointer<SoapCallDouble>, SoapCallWithError);
    QFETCH(PendingSoapCall::ErrorCode, ExpectedErrorCode);

    PendingSoapCall pendingSoapCall
    {
        SoapCallWithError
    };

    SoapCallWithError->finished();

    QVERIFY2(ExpectedErrorCode == pendingSoapCall.errorCode(),
             QString{"Expected: %1\n Actual: %2"}
                .arg(QVariant::fromValue(ExpectedErrorCode).toString())
                .arg(QVariant::fromValue(pendingSoapCall.errorCode()).toString())
             .toLocal8Bit());
}

void PendingSoapCallShould::give_Error_Description_When_Call_Finished_With_Error()
{
    QString errorMessage = QString{errorXML}.arg(600 /*Action Failed */).arg("ActionFailed");
    QSharedPointer<SoapCallDouble> soapCallDouble
    {
        new SoapCallDouble()
    };
    soapCallDouble->setErrorState(true);
    soapCallDouble->setRawMessage(errorMessage);
    QString expectedErrorDescription{"ActionFailed"};

    PendingSoapCall pendingSoapCall
    {
        soapCallDouble
    };

    soapCallDouble->finished();
    auto errorDesc = pendingSoapCall.errorDescription();

    QVERIFY2(expectedErrorDescription == errorDesc,
             QString{"Expected %1 Actual %2"}.arg(expectedErrorDescription).arg(errorDesc).toLocal8Bit());
}

void
PendingSoapCallShould::return_An_Object_That_is_Constructable_With_QString_That_Returns_The_Message_When_Call_Finished()
{
    QSharedPointer<SoapCallDouble> soapCallDouble{new SoapCallDouble{}};
    soapCallDouble->setErrorState(false);
    soapCallDouble->setRawMessage("Hello from SOAP call.");
    QString expectedString{"Hello from SOAP call."};
    PendingSoapCall pendingSoapCall{soapCallDouble};

    soapCallDouble->finished();
    auto testObject = pendingSoapCall.resultAs<TestObject>();

    QVERIFY2(expectedString == testObject->rawMessage(),
             QString{"Expected: %1 \nActual: %2"}
                .arg(expectedString)
                .arg(testObject->rawMessage())
                .toLocal8Bit());
}

} //namespace UPnPAV

QTEST_MAIN(UPnPAV::PendingSoapCallShould);
