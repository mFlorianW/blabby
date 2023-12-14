#include "MediaServerObjectBuilder.hpp"

namespace UPnPAV
{

MediaServerObjectBuilder &MediaServerObjectBuilder::withId(QString const &id) noexcept
{
    mObj.mId = id;
    return *this;
}

MediaServerObjectBuilder &MediaServerObjectBuilder::withParentId(QString const &parentId) noexcept
{
    mObj.mParentId = parentId;
    return *this;
}

MediaServerObjectBuilder &MediaServerObjectBuilder::withTitle(QString const &title) noexcept
{
    mObj.mTitle = title;
    return *this;
}

MediaServerObjectBuilder &MediaServerObjectBuilder::withTypeClass(QString const &typeClass) noexcept
{
    mObj.mClass = typeClass;
    return *this;
}

MediaServerObjectBuilder &MediaServerObjectBuilder::withPlayUrl(QString const &playUrl) noexcept
{
    mObj.mPlayUrl = playUrl;
    return *this;
}
MediaServerObjectBuilder &MediaServerObjectBuilder::withSupportedProtocols(
    QStringList const &supportedProtocols) noexcept
{
    mObj.mSupportedProtocols = supportedProtocols;
    return *this;
}

bool MediaServerObjectBuilder::isValid() const noexcept
{
    return not mObj.mId.isEmpty() and not mObj.mParentId.isEmpty() and not mObj.mTitle.isEmpty() and
           not mObj.mTitle.isEmpty();
}

MediaServerObject MediaServerObjectBuilder::build()
{
    return mObj;
}

} // namespace UPnPAV
