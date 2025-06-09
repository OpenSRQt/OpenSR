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

#ifndef OPENSR_WORLD_DESERTPLANET_H
#define OPENSR_WORLD_DESERTPLANET_H

#include "Planet.h"
#include "World.h"
#include <QGraphicsItem>

namespace OpenSR
{
namespace World
{

class OPENSR_WORLD_API DesertPlanetStyle : public Resource
{
    Q_GADGET

    Q_PROPERTY(QString surface READ surface WRITE setSurface)
    Q_PROPERTY(QString cloud0 READ cloud0 WRITE setCloud0)
    Q_PROPERTY(QString cloud1 READ cloud1 WRITE setCloud1)
    Q_PROPERTY(int radius READ radius WRITE setRadius)
    Q_PROPERTY(QColor atmosphere READ atmosphere WRITE setAtmosphere)
    Q_PROPERTY(QString background READ background WRITE setBackground)

public:
    PlanetStyle planetStyle;
    struct Data
    {
    };

    QString surface() const;
    QString cloud0() const;
    QString cloud1() const;
    int radius() const;
    QColor atmosphere() const;
    QString background() const;

    void setSurface(const QString &);
    void setCloud0(const QString &);
    void setCloud1(const QString &);
    void setRadius(int);
    void setAtmosphere(const QColor &);
    void setBackground(const QString &);
};

bool operator==(const DesertPlanetStyle &one, const DesertPlanetStyle &another);

QDataStream &operator<<(QDataStream &stream, const DesertPlanetStyle &style);
QDataStream &operator>>(QDataStream &stream, DesertPlanetStyle &style);
QDataStream &operator<<(QDataStream &stream, const DesertPlanetStyle::Data &data);
QDataStream &operator>>(QDataStream &stream, DesertPlanetStyle::Data &data);

class OPENSR_WORLD_API DesertPlanet : public Planet
{
    Q_OBJECT
    OPENSR_WORLD_OBJECT

    Q_PROPERTY(OpenSR::World::DesertPlanetStyle style READ style WRITE setStyle NOTIFY styleChanged)
public:
    Q_INVOKABLE DesertPlanet(WorldObject *parent = 0, quint32 id = 0);
    ~DesertPlanet() override;

    quint32 typeId() const override;
    QString namePrefix() const override;

    DesertPlanetStyle style() const;
    int radius() override;
    void setStyle(const DesertPlanetStyle &style);

Q_SIGNALS:
    void styleChanged();

private:
    DesertPlanetStyle m_style;
};
} // namespace World
} // namespace OpenSR

Q_DECLARE_METATYPE(OpenSR::World::DesertPlanetStyle::Data)
Q_DECLARE_METATYPE(OpenSR::World::DesertPlanetStyle)

#endif // OPENSR_WORLD_DESERTPLANET_H
