#include "MediaServerObjectBuilder.hpp"

namespace UPnPAV
{

MediaServerObjectBuilder &MediaServerObjectBuilder::withId(QString const &id) noexcept
{
    mObj.m_id = id;
    return *this;
}

MediaServerObjectBuilder &MediaServerObjectBuilder::withParentId(QString const &parentId) noexcept
{
    mObj.m_parentId = parentId;
    return *this;
}

MediaServerObjectBuilder &MediaServerObjectBuilder::withTitle(QString const &title) noexcept
{
    mObj.m_title = title;
    return *this;
}

MediaServerObjectBuilder &MediaServerObjectBuilder::withTypeClass(QString const &typeClass) noexcept
{
    mObj.m_class = typeClass;
    return *this;
}

MediaServerObjectBuilder &MediaServerObjectBuilder::withPlayUrl(QString const &playUrl) noexcept
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
