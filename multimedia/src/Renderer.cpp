#include "Renderer.hpp"

namespace Multimedia
{

Renderer::Renderer(std::unique_ptr<UPnPAV::MediaRenderer> mediaRenderer)
    : mRenderer{std::move(mediaRenderer)}
{
}

} // namespace Multimedia
