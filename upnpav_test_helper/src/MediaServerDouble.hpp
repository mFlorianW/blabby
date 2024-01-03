// Copyright 2020 Florian Weßel <florianwessel@gmx.net>.
// SPDX-FileCopyrightText: 2021 - 2023 Florian Weßel <florianwessel@gmx.net>
// SPDX-FileCopyrightText: 2024 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "DeviceDescription.hpp"
#include "MediaServer.hpp"
#include "SoapCallDouble.hpp"
#include <QUrl>

namespace UPnPAV::Doubles
{

class MediaServerFactory final : public UPnPAV::MediaServerFactory
{
public:
    quint32 howOftenCalled{0};
    std::unique_ptr<UPnPAV::MediaServer> createMediaServer(const UPnPAV::DeviceDescription &deviceDescription) override;
};

struct LastBrowseRequest
{
    QString objectId{""};
    UPnPAV::MediaServer::BrowseFlag browseFlag{UPnPAV::MediaServer::BrowseFlag::MetaData};

    bool operator==(const LastBrowseRequest &rhs) const noexcept
    {
        return (objectId == rhs.objectId) && (browseFlag == rhs.browseFlag);
    }

    bool operator!=(const LastBrowseRequest &rhs) const noexcept
    {
        return !operator==(rhs);
    }
};

class MediaServer final : public UPnPAV::MediaServer
{
    Q_DISABLE_COPY_MOVE(MediaServer)
public:
    MediaServer();
    ~MediaServer() override;

    const QString &name() const noexcept override;

    const QUrl &iconUrl() const noexcept override;

    void setIconUrl(QString const &iconUrl) noexcept;

    std::unique_ptr<UPnPAV::PendingSoapCall> protocolInfo() noexcept override;

    std::unique_ptr<UPnPAV::PendingSoapCall> currentConnectionIds() noexcept override;

    std::unique_ptr<UPnPAV::PendingSoapCall> currentConnectionInfo(quint32 connectionId) noexcept override;

    std::unique_ptr<UPnPAV::PendingSoapCall> getSortCapabilities() noexcept override;

    std::unique_ptr<UPnPAV::PendingSoapCall> browse(const QString &objectId,
                                                    BrowseFlag browseFlag,
                                                    const QString &filter,
                                                    const QString &sortCriteria) noexcept override;

public:
    LastBrowseRequest lastBrowseRequest{};
    QSharedPointer<UPnPAV::SoapCallDouble> soapCall{nullptr};
    QString mName{"MediaServer"};
    QUrl mIconUrl{"http://localhost:8200/icons/sm.png"};
};

} // namespace UPnPAV::Doubles
