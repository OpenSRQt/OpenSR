#include "GAIShader.h"
#include "GAIMaterial.h"
#include <QFile>

namespace OpenSR
{
GAIShader::GAIShader()
{
    setShaderFileName(VertexStage, QStringLiteral("://shaders/GAIShader.vert.qsb"));
    setShaderFileName(FragmentStage, QStringLiteral("://shaders/GAIShader.frag.qsb"));
}

bool GAIShader::updateUniformData(RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial)
{
    bool changed = false;
    QByteArray *buf = state.uniformData();

    if (state.isMatrixDirty())
    {
        const QMatrix4x4 m = state.combinedMatrix();
        memcpy(buf->data(), m.constData(), 64);
        changed = true;
    }

    return changed;
}

void GAIShader::updateSampledImage(RenderState &state, int binding, QSGTexture **texture, QSGMaterial *newMaterial,
                                   QSGMaterial *oldMaterial)
{
    GAIMaterial *mat = static_cast<GAIMaterial *>(newMaterial);
    if (binding == 1)
    {
        mat->texture()->commitTextureOperations(state.rhi(), state.resourceUpdateBatch());
        *texture = mat->texture();
    }
}
} // namespace OpenSR