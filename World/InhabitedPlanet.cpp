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

#include "InhabitedPlanet.h"
#include "Planet.h"
#include "ResourceManager.h"

#include <QtQml>

namespace OpenSR
{
namespace World
{

QString InhabitedPlanetStyle::surface() const
{
    return planetStyle.surface();
}

void InhabitedPlanetStyle::setSurface(const QString &texture)
{
    planetStyle.setSurface(texture);
}

QString InhabitedPlanetStyle::cloud0() const
{
    return planetStyle.cloud0();
}

void InhabitedPlanetStyle::setCloud0(const QString &texture)
{
    planetStyle.setCloud0(texture);
}

QString InhabitedPlanetStyle::cloud1() const
{
    return planetStyle.cloud1();
}

void InhabitedPlanetStyle::setCloud1(const QString &texture)
{
    planetStyle.setCloud1(texture);
}

int InhabitedPlanetStyle::radius() const
{
    return planetStyle.radius();
}

void InhabitedPlanetStyle::setRadius(const int r)
{
    planetStyle.setRadius(r);
}

QColor InhabitedPlanetStyle::atmosphere() const
{
    return planetStyle.atmosphere();
}

void InhabitedPlanetStyle::setAtmosphere(const QColor &c)
{
    planetStyle.setAtmosphere(c);
}

QString InhabitedPlanetStyle::background() const
{
    return planetStyle.background();
}

void InhabitedPlanetStyle::setBackground(const QString &texture)
{
    planetStyle.setBackground(texture);
}

QString InhabitedPlanetStyle::affiliation() const
{
    return getData<Data>().affiliation;
}

void InhabitedPlanetStyle::setAffiliation(const QString &texture)
{
    auto d = getData<Data>();
    d.affiliation = texture;
    setData(d);
}


bool operator==(const InhabitedPlanetStyle &one, const InhabitedPlanetStyle &another)
{
    return (one.planetStyle == another.planetStyle) &&
            (one.affiliation() == another.affiliation());
}

QDataStream &operator<<(QDataStream &stream, const InhabitedPlanetStyle &style)
{
    return stream << style.id();
}

QDataStream &operator>>(QDataStream &stream, InhabitedPlanetStyle &style)
{
    quint32 id;
    stream >> id;
    ResourceManager *m = ResourceManager::instance();
    Q_ASSERT(m != 0);
    Resource::replaceData(style, m->getResource(id));
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const InhabitedPlanetStyle::Data &data)
{
    return stream << data.affiliation;
}

QDataStream &operator>>(QDataStream &stream, InhabitedPlanetStyle::Data &data)
{
    return stream >> data.affiliation;
}

const quint32 InhabitedPlanet::m_staticTypeId = typeIdFromClassName(InhabitedPlanet::staticMetaObject.className());

template <>
void WorldObject::registerType<InhabitedPlanet>(QQmlEngine *qml, QJSEngine *script)
{
    qRegisterMetaType<InhabitedPlanetStyle>();
    qRegisterMetaTypeStreamOperators<InhabitedPlanetStyle>();
    qRegisterMetaType<InhabitedPlanetStyle::Data>();
    qRegisterMetaTypeStreamOperators<InhabitedPlanetStyle::Data>();
    qmlRegisterType<InhabitedPlanet>("OpenSR.World", 1, 0, "InhabitedPlanet");
}

template <>
InhabitedPlanet *WorldObject::createObject(WorldObject *parent, quint32 id)
{
    return new InhabitedPlanet(parent, id);
}

template <>
quint32 WorldObject::staticTypeId<InhabitedPlanet>()
{
    return InhabitedPlanet::m_staticTypeId;
}

template <>
const QMetaObject *WorldObject::staticTypeMeta<InhabitedPlanet>()
{
    return &InhabitedPlanet::staticMetaObject;
}

InhabitedPlanet::InhabitedPlanet(WorldObject *parent, quint32 id) : Planet(parent, id)
{
}

InhabitedPlanet::~InhabitedPlanet()
{
}

quint32 InhabitedPlanet::typeId() const
{
    return InhabitedPlanet::m_staticTypeId;
}

QString InhabitedPlanet::namePrefix() const
{
    return tr("Inhabited planet");
}

InhabitedPlanetStyle InhabitedPlanet::style() const
{
    return m_style;
}

void InhabitedPlanet::setStyle(const InhabitedPlanetStyle &style)
{
    if (m_style == style)
        return;

    m_style = style;
    emit styleChanged();
}

void InhabitedPlanet::prepareSave()
{
    WorldObject::prepareSave();
    m_style.registerResource();
}

int InhabitedPlanet::radius() {
    return style().radius();
}


} //namespace World
} //namespace OpenSR
