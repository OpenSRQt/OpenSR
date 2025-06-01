#include "GAIMaterial.h"
#include "GAIShader.h"

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
    return m_texture;
}

void GAIMaterial::setTexture(GAITexture *texture)
{
    m_texture = texture;
}

} // namespace OpenSR