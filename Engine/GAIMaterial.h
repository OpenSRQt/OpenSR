#pragma once

#include <OpenSR/GAITexture.h>
#include <QSGMaterial>

namespace OpenSR
{
class GAIMaterial : public QSGMaterial
{
public:
    GAIMaterial(GAITexture *texture = nullptr);
    ~GAIMaterial() override = default;

    QSGMaterialType *type() const override;
    QSGMaterialShader *createShader(QSGRendererInterface::RenderMode) const override;
    GAITexture *texture() const;
    void setTexture(std::shared_ptr<GAITexture> texture);

private:
    std::shared_ptr<GAITexture> m_texture;
};
} // namespace OpenSR