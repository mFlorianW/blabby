#include "MediaServerObjectBuilder.hpp"

namespace UPnPAV
{

MediaServerObjectBuilder &MediaServerObjectBuilder::setId(QString const &id) noexcept
{
    mObj.m_id = id;
    return *this;
}

MediaServerObjectBuilder &MediaServerObjectBuilder::setParentId(QString const &parentId) noexcept
{
    mObj.m_parentId = parentId;
    return *this;
}

MediaServerObjectBuilder &MediaServerObjectBuilder::setTitle(QString const &title) noexcept
{
    mObj.m_title = title;
    return *this;
}

MediaServerObjectBuilder &MediaServerObjectBuilder::setTypeClass(QString const &typeClass) noexcept
{
    mObj.m_class = typeClass;
    return *this;
}

MediaServerObjectBuilder &MediaServerObjectBuilder::setPlayUrl(QString const &playUrl) noexcept
{
    mObj.m_playUrl = playUrl;
    return *this;
}

bool MediaServerObjectBuilder::isValid() const noexcept
{
    return not mObj.m_id.isEmpty() and not mObj.m_parentId.isEmpty() and not mObj.m_title.isEmpty() and
           not mObj.m_title.isEmpty();
}

MediaServerObject MediaServerObjectBuilder::build()
{
    return mObj;
}

} // namespace UPnPAV
