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

#include "DesertPlanet.h"
#include "Planet.h"
#include "ResourceManager.h"

#include <QtQml>

namespace OpenSR
{
namespace World
{

QString DesertPlanetStyle::surface() const
{
    return planetStyle.surface();
}

void DesertPlanetStyle::setSurface(const QString &texture)
{
    planetStyle.setSurface(texture);
}

QString DesertPlanetStyle::cloud0() const
{
    return planetStyle.cloud0();
}

void DesertPlanetStyle::setCloud0(const QString &texture)
{
    planetStyle.setCloud0(texture);
}

QString DesertPlanetStyle::cloud1() const
{
    return planetStyle.cloud1();
}

void DesertPlanetStyle::setCloud1(const QString &texture)
{
    planetStyle.setCloud1(texture);
}

int DesertPlanetStyle::radius() const
{
    return planetStyle.radius();
}

void DesertPlanetStyle::setRadius(const int r)
{
    planetStyle.setRadius(r);
}

QColor DesertPlanetStyle::atmosphere() const
{
    return planetStyle.atmosphere();
}

void DesertPlanetStyle::setAtmosphere(const QColor &c)
{
    planetStyle.setAtmosphere(c);
}

QString DesertPlanetStyle::background() const
{
    return planetStyle.background();
}

void DesertPlanetStyle::setBackground(const QString &texture)
{
    planetStyle.setBackground(texture);
}

bool operator==(const DesertPlanetStyle &one, const DesertPlanetStyle &another)
{
    return (one.planetStyle == another.planetStyle);
}

QDataStream &operator<<(QDataStream &stream, const DesertPlanetStyle &style)
{
    return stream << style.id();
}

QDataStream &operator>>(QDataStream &stream, DesertPlanetStyle &style)
{
    quint32 id;
    stream >> id;
    ResourceManager *m = ResourceManager::instance();
    Q_ASSERT(m != 0);
    Resource::replaceData(style, m->getResource(id));
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const DesertPlanetStyle::Data &)
{
    return stream;
}

QDataStream &operator>>(QDataStream &stream, DesertPlanetStyle::Data &)
{
    return stream;
}

const quint32 DesertPlanet::m_staticTypeId = typeIdFromClassName(DesertPlanet::staticMetaObject.className());

template <>
void WorldObject::registerType<DesertPlanet>(QQmlEngine *qml, QJSEngine *script)
{
    qRegisterMetaType<DesertPlanetStyle>();
    qRegisterMetaType<DesertPlanetStyle::Data>();
    qmlRegisterType<DesertPlanet>("OpenSR.World", 1, 0, "DesertPlanet");
}

template <>
DesertPlanet *WorldObject::createObject(WorldObject *parent, quint32 id)
{
    return new DesertPlanet(parent, id);
}

template <>
quint32 WorldObject::staticTypeId<DesertPlanet>()
{
    return DesertPlanet::m_staticTypeId;
}

template <>
const QMetaObject *WorldObject::staticTypeMeta<DesertPlanet>()
{
    return &DesertPlanet::staticMetaObject;
}

DesertPlanet::DesertPlanet(WorldObject *parent, quint32 id) : Planet(parent, id)
{
}

DesertPlanet::~DesertPlanet()
{
}

quint32 DesertPlanet::typeId() const
{
    return DesertPlanet::m_staticTypeId;
}

QString DesertPlanet::namePrefix() const
{
    return tr("Desert planet");
}

DesertPlanetStyle DesertPlanet::style() const
{
    return m_style;
}

void DesertPlanet::setStyle(const DesertPlanetStyle &style)
{
    if (m_style == style)
        return;

    m_style = style;
    emit styleChanged();
}

int DesertPlanet::radius() {
    return style().radius();
}
} // namespace World
} // namespace OpenSR
