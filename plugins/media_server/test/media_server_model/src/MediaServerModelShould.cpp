#include "MediaServerModelShould.h"
#include "IMediaServer.h"
#include "MediaServerModel.h"
#include "PendingSoapCall.h"
#include <QtTest>

namespace MediaServerPlugin
{

class MediaServer : public UPnPAV::IMediaServer
{
public:
    QString name() const noexcept override
    {
        return "MediaServer";
    }

    QUrl iconUrl() const noexcept override
    {
        return QUrl{ "http://localhost:8200/icons/sm.png" };
    }

    QSharedPointer<UPnPAV::PendingSoapCall> getSortCapabilities() noexcept override
    {
        return {};
    }

    QSharedPointer<UPnPAV::PendingSoapCall> browse(const QString &objectId, BrowseFlag browseFlag,
                                                   const QString &filter, const QString &sortCriteria) noexcept override
    {
        Q_UNUSED(objectId)
        Q_UNUSED(browseFlag)
        Q_UNUSED(filter)
        Q_UNUSED(sortCriteria)

        return {};
    }
};

MediaServerModelShould::MediaServerModelShould()
    : QObject()
{
}

void MediaServerModelShould::give_correct_rolename()
{
    QHash<int, QByteArray> expectedRoleNames{ std::make_pair(MediaServerModel::MediaServerName, "mediaServerName"),
                                              std::make_pair(MediaServerModel::MediaServerIconUrl,
                                                             "mediaServerIconUrl") };
    MediaServerModel mediaServerModel;

    auto roleNames = mediaServerModel.roleNames();

    QCOMPARE(roleNames, expectedRoleNames);
}

void MediaServerModelShould::give_mediaserver_name_on_request_with_valid_index()
{
    MediaServer mediaServer;
    MediaServerModel mediaServerModel;
    mediaServerModel.insert(&mediaServer);
    const QString expected{ "MediaServer" };

    auto mediaServerName = mediaServerModel.data(mediaServerModel.index(0), MediaServerModel::MediaServerName).toString();

    QCOMPARE(mediaServerName, expected);
}

void MediaServerModelShould::give_mediaserver_icon_url_on_request_with_valid_index()
{
    MediaServer mediaServer;
    MediaServerModel mediaServerModel;
    mediaServerModel.insert(&mediaServer);
    const QUrl expected{ "http://localhost:8200/icons/sm.png" };

    auto mediaServerName = mediaServerModel.data(mediaServerModel.index(0), MediaServerModel::MediaServerIconUrl).toUrl();

    QCOMPARE(mediaServerName, expected);
}

void MediaServerModelShould::give_default_qvariant_for_a_to_small_index()
{
    MediaServerModel mediaServerModel;

    auto emptyVariant = mediaServerModel.data(mediaServerModel.index(-1), MediaServerModel::MediaServerName);

    QCOMPARE(emptyVariant, QVariant{});

    emptyVariant = mediaServerModel.data(mediaServerModel.index(-1), MediaServerModel::MediaServerName);
}

void MediaServerModelShould::give_default_qvariant_for_a_to_big_index()
{
    MediaServerModel mediaServerModel;

    auto emptyVariant = mediaServerModel.data(mediaServerModel.index(19), MediaServerModel::MediaServerName);

    QCOMPARE(emptyVariant, QVariant{});

    emptyVariant = mediaServerModel.data(mediaServerModel.index(-1), MediaServerModel::MediaServerName);
}

} // namespace MediaServerPlugin

QTEST_MAIN(MediaServerPlugin::MediaServerModelShould)
