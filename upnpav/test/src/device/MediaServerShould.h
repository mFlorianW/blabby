#ifndef MEDIASERVERSHOULD_H
#define MEDIASERVERSHOULD_H

#include <QObject>

namespace UPnPAV
{

class MediaServerShould : public QObject
{
    Q_OBJECT
public:
    explicit MediaServerShould();

private Q_SLOTS:
    /**
     * @test The media server shall be only createable when the device description contains
     * the required service content directory.
     */
    void be_Creatable_Only_With_ServiceDescription_ContentDirectory();

    /**
     * @test The media server shall be only createable when the device description contains
     * the required service content directory.
     */
    void be_Creatable_Only_With_ServiceDescription_ConnectionManager();

    /**
     * @test The media server shall be only createable when the passed content directory
     * description contains an not empty event url.
     */
    void be_Creatable_Only_With_A_Non_Empty_EventUrl_In_ContentDirectory_Description();

    /**
     * @test The media server shall be only createable when the content directory description
     * has a control url set.
     */
    void be_Creatable_Only_With_A_Non_Empty_ContrlUrl_In_ContentDirectory_Description();

    /**
     * @test The media server shall be only createable when the content directory description
     * has a service id set.
     */
    void be_Creatable_Only_With_A_Non_Empty_ServiceId_In_ContentDirectory_Description();


};

} //namespace UPnPAV

#endif // MEDIASERVERSHOULD_H
