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
     * @test The media server shall be only instantiateable when the device description contains
     * the required services connection manager and content directory.
     */
    void beInstantiateableOnlyWithRequiredServiceDescription();
};

} //namespace UPnPAV

#endif // MEDIASERVERSHOULD_H
