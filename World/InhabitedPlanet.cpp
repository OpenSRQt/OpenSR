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
#include "ResourceManager.h"
#include "WorldContext.h"

#include <QtQml>

namespace OpenSR
{
namespace World
{

QString InhabitedPlanetStyle::surface() const
{
    return getData<Data>().surface;
}

void InhabitedPlanetStyle::setSurface(const QString &texture)
{
    auto d = getData<Data>();
    d.surface = texture;
    setData(d);
}

QString InhabitedPlanetStyle::cloud0() const
{
    return getData<Data>().cloud0;
}

void InhabitedPlanetStyle::setCloud0(const QString &texture)
{
    auto d = getData<Data>();
    d.cloud0 = texture;
    setData(d);
}

QString InhabitedPlanetStyle::cloud1() const
{
    return getData<Data>().cloud1;
}

void InhabitedPlanetStyle::setCloud1(const QString &texture)
{
    auto d = getData<Data>();
    d.cloud1 = texture;
    setData(d);
}

int InhabitedPlanetStyle::radius() const
{
    return getData<Data>().radius;
}

void InhabitedPlanetStyle::setRadius(const int r)
{
    auto d = getData<Data>();
    d.radius = r;
    setData(d);
}

QColor InhabitedPlanetStyle::atmosphere() const
{
    return getData<Data>().atmosphere;
}

void InhabitedPlanetStyle::setAtmosphere(const QColor& c)
{
    auto d = getData<Data>();
    d.atmosphere = c;
    setData(d);
}

QString InhabitedPlanetStyle::background() const 
{
    return getData<Data>().background;
}

void InhabitedPlanetStyle::setBackground(const QString& texture)
{
    auto d = getData<Data>();
    d.background = texture;
    setData(d);
}

bool operator==(const InhabitedPlanetStyle& one, const InhabitedPlanetStyle& another)
{
    return  (one.surface()  == another.surface() ) &&
        (one.cloud0() == another.cloud0()) &&
        (one.cloud1() == another.cloud1()) &&
        (one.radius() == another.radius()) &&
        (one.atmosphere() == another.atmosphere()) &&
        (one.background() == another.background())
        ;
}

QDataStream& operator<<(QDataStream & stream, const InhabitedPlanetStyle& style)
{
    return stream << style.id();
}

QDataStream& operator>>(QDataStream & stream, InhabitedPlanetStyle& style)
{
    quint32 id;
    stream >> id;
    ResourceManager *m = ResourceManager::instance();
    Q_ASSERT(m != 0);
    Resource::replaceData(style, m->getResource(id));
    return stream;
}

QDataStream& operator<<(QDataStream & stream, const InhabitedPlanetStyle::Data& data)
{
    return stream << data.surface << data.cloud0 << data.cloud1 << data.radius << data.background;
}

QDataStream& operator>>(QDataStream & stream, InhabitedPlanetStyle::Data& data)
{
    return stream >> data.surface >> data.cloud0 >> data.cloud1 >> data.radius >> data.background;
}

const quint32 InhabitedPlanet::m_staticTypeId = typeIdFromClassName(InhabitedPlanet::staticMetaObject.className());

template<>
void WorldObject::registerType<InhabitedPlanet>(QQmlEngine *qml, QJSEngine *script)
{
    qRegisterMetaType<InhabitedPlanetStyle>();
    qRegisterMetaTypeStreamOperators<InhabitedPlanetStyle>();
    qRegisterMetaType<InhabitedPlanetStyle::Data>();
    qRegisterMetaTypeStreamOperators<InhabitedPlanetStyle::Data>();
    qmlRegisterType<InhabitedPlanet>("OpenSR.World", 1, 0, "InhabitedPlanet");
}

template<>
InhabitedPlanet* WorldObject::createObject(WorldObject *parent, quint32 id)
{
    return new InhabitedPlanet(parent, id);
}

template<>
quint32 WorldObject::staticTypeId<InhabitedPlanet>()
{
    return InhabitedPlanet::m_staticTypeId;
}

template<>
const QMetaObject* WorldObject::staticTypeMeta<InhabitedPlanet>()
{
    return &InhabitedPlanet::staticMetaObject;
}

InhabitedPlanet::InhabitedPlanet(WorldObject *parent, quint32 id): Planet(parent, id)
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

void InhabitedPlanet::setStyle(const InhabitedPlanetStyle& style)
{
    if (m_style == style)
        return;

    m_style = style;
    emit(styleChanged());
}

void InhabitedPlanet::prepareSave()
{
    WorldObject::prepareSave();
    m_style.registerResource();
}

void InhabitedPlanet::waitForArrival()
{
    emit(planetToEnter());
}

}
}
