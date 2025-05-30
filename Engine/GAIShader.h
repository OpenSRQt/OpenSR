#pragma once

#include <QSGMaterialShader>

namespace OpenSR
{
class GAIShader : public QSGMaterialShader
{
public:
    GAIShader();

    bool updateUniformData(RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial) override;
    void updateSampledImage(RenderState &state, int binding, QSGTexture **texture, QSGMaterial *newMaterial,
                            QSGMaterial *oldMaterial) override;
};
} // namespace OpenSR