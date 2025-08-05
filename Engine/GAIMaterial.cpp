#include "GAIMaterial.h"
#include "GAIShader.h"
#include <memory>

namespace OpenSR
{
GAIMaterial::GAIMaterial(GAITexture *texture) : m_texture(texture)
{
    setFlag(Blending, true);
}

QSGMaterialType *GAIMaterial::type() const
{
    static QSGMaterialType type;
    return &type;
}

QSGMaterialShader *GAIMaterial::createShader(QSGRendererInterface::RenderMode) const
{
    return new GAIShader();
}

GAITexture *GAIMaterial::texture() const
{
    return m_texture.get();
}

void GAIMaterial::setTexture(std::shared_ptr<GAITexture> texture)
{
    m_texture = texture;
}

} // namespace OpenSR