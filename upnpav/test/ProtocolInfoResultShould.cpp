#include "ProtocolInfoResultShould.h"
#include "ProtocolInfoResponse.h"
#include "ProtocolInfoResult.h"
#include <QTest>
#include <qtestcase.h>
#include <qvector.h>

namespace UPnPAV
{

void ProtocolInfoResultShould::give_the_protocol_lists()
{
    auto pInfo = ProtocolInfoResult{ValidProtoclInfoResponse};
    auto expectedResult = QVector<Protocol>{
        Protocol{.protocol = "file", .network = "*", .contentFormat = "audio/mpegurl", .additionalInfo = "*"},
        Protocol{.protocol = "x-file-cifs", .network = "*", .contentFormat = "*", .additionalInfo = "*"},
        Protocol{.protocol = "x-rincon", .network = "*", .contentFormat = "*", .additionalInfo = "*"},
        Protocol{.protocol = "x-rincon-mp3radio", .network = "*", .contentFormat = "*", .additionalInfo = "*"},
        Protocol{.protocol = "x-rincon-playlist", .network = "*", .contentFormat = "*", .additionalInfo = "*"},
        Protocol{.protocol = "x-rincon-queue", .network = "*", .contentFormat = "*", .additionalInfo = "*"},
        Protocol{.protocol = "x-rincon-stream", .network = "*", .contentFormat = "*", .additionalInfo = "*"},
    };

    QVERIFY2(pInfo.getSourceProtocols() == expectedResult,
             "The returned result doesn't match expected source protocol result");
    QVERIFY2(pInfo.getSinkProtocols() == expectedResult,
             "The returned result doesn't match expected sink protocol result");
}

} // namespace UPnPAV

QTEST_MAIN(UPnPAV::ProtocolInfoResultShould)
