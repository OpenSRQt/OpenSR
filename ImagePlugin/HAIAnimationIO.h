/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2014 Kosyak <ObKo@mail.ru>

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

#ifndef OPENSR_HAI_ANIMATION_IO_H
#define OPENSR_HAI_ANIMATION_IO_H

#include <OpenSR/libRangerQt.h>
#include <QImage>
#include <QImageIOHandler>
#include <QVector>

namespace OpenSR
{
class HAIAnimationIO : public QImageIOHandler
{
public:
    HAIAnimationIO();
    ~HAIAnimationIO() override;

    bool canRead() const override;
    bool read(QImage *image) override;

    bool supportsOption(ImageOption option) const override;
    QVariant option(ImageOption option) const override;

    int imageCount() const override;
    int loopCount() const override;
    int nextImageDelay() const override;
    int currentImageNumber() const override;

private:
    HAIHeader m_header{};
    int m_currentFrame;
};
} // namespace OpenSR

#endif // OPENSR_HAI_ANIMATION_IO_H
