/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2015 Kosyak <ObKo@mail.ru>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "OpenSR/GAITexture.h"

#include <OpenSR/libRangerQt.h>
#include <QBuffer>
#include <QDebug>
#include <QImage>

namespace OpenSR
{
GAITexture::GAITexture(const GAIHeader &header, const QImage &background)
    : QSGTexture(), m_texID(0), m_header(header), m_needDraw(false), m_bg(background)
{
    m_size = QSize(m_header.finishX - m_header.startX, m_header.finishY - m_header.startY);
    reset();
}

GAITexture::~GAITexture()
{
    if (m_texture)
    {
        m_texture->destroy();
        delete m_texture;
    }
}

void GAITexture::drawNextFrame(const QByteArray &frameData, const QPoint &start)
{
    m_nextFrameData = frameData;
    m_decodeStart = start;
    m_needDraw = true;
}

void GAITexture::reset()
{
    m_nextFrameData = QByteArray();
    m_needDraw = true;
}

void GAITexture::commitTextureOperations(QRhi *rhi, QRhiResourceUpdateBatch *resourceUpdates)
{
    // QOpenGLFunctions *gl = QOpenGLContext::currentContext()->functions();

    // if (m_texID && !m_needDraw)
    // {
    //     gl->glBindTexture(GL_TEXTURE_2D, m_texID);
    //     updateBindOptions();
    //     return;
    // }

    // if (!m_texID)
    // {
    //     gl->glGenTextures(1, (GLuint *)&m_texID);
    //     gl->glBindTexture(GL_TEXTURE_2D, m_texID);
    //     gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.width(), m_size.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
    //     0); updateBindOptions(true);
    // }
    //qDebug() << "commitTextureOperations()";

    if (!m_texture)
    {
        m_texture = rhi->newTexture(QRhiTexture::RGBA8, m_size, 1, QRhiTexture::UsedAsTransferSource);
        m_texture->create();
    }

    if (m_needDraw)
    {
        if (m_nextFrameData.isEmpty())
        {
            if (!m_bg.isNull())
            {
                //qDebug() << "commitTextureOperations(): bg";
                resourceUpdates->uploadTexture(m_texture, m_bg);
            }
        }
        else
        {
            //qDebug() << "commitTextureOperations(): texture";
            QImage img(m_size, QImage::Format_RGBA8888);
            QBuffer d(&m_nextFrameData);
            d.open(QIODevice::ReadOnly);
            decodeGAIDeltaFrame(img, m_decodeStart.x(), m_decodeStart.y(), &d);
            d.close();

            resourceUpdates->uploadTexture(m_texture, img);
        }
        m_needDraw = false;
        //rhi->finish(); // ?
    }
}

bool GAITexture::hasAlphaChannel() const
{
    return true;
}

bool GAITexture::hasMipmaps() const
{
    return false;
}

qint64 GAITexture::comparisonKey() const
{
    return qint64(m_texture);
}

QSize GAITexture::textureSize() const
{
    return m_size;
}

QRhiTexture *GAITexture::rhiTexture() const
{
    if (m_texture)
    {
        return m_texture;
    }
    return nullptr;
}
} // namespace OpenSR