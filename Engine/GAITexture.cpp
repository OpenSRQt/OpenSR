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
#include <memory>

namespace OpenSR
{
GAITexture::GAITexture(const GAIHeader &header, const QImage &background)
    : m_header(header), m_needDraw(true), m_bg(background),
      m_size(QSize(m_header.finishX - m_header.startX, m_header.finishY - m_header.startY)),
      m_nextFrameData(QByteArray())
{
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

void GAITexture::commitTextureOperations(QRhi *rhi, QRhiResourceUpdateBatch *resourceUpdates)
{
    if (!imgBuffer)
    {
        imgBuffer = std::make_unique<uchar[]>(m_size.width() * m_size.height() * 4);
    }

    if (!m_texture)
    {
        m_texture = rhi->newTexture(QRhiTexture::RGBA8, m_size, 1, QRhiTexture::UsedAsTransferSource);
        m_texture->create();
    }

    if (m_needDraw)
    {
        QImage img;
        if (m_nextFrameData.isEmpty())
        {
            if (!m_bg.isNull())
            {
                memcpy(imgBuffer.get(), m_bg.constBits(), m_size.width() * m_size.height() * 4);
                img = QImage(imgBuffer.get(), m_size.width(), m_size.height(), QImage::Format_RGBA8888);
            }
            else
            {
                return;
            }
        }
        else
        {
            img = QImage(imgBuffer.get(), m_size.width(), m_size.height(), QImage::Format_RGBA8888);
            QBuffer d(&m_nextFrameData);
            d.open(QIODevice::ReadOnly);
            decodeGAIDeltaFrame(img, m_decodeStart.x(), m_decodeStart.y(), &d);
            d.close();
        }
        resourceUpdates->uploadTexture(m_texture, img);
        m_needDraw = false;
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