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

#include "WorldContext.h"
#include "Asteroid.h"
#include "Asteroid.h"
#include "Ship.h"
#include "WorldObject.h"
#include <QDataStream>
#include <QMap>
#include <QtQml>

namespace OpenSR
{
namespace World
{
template <> void WorldObject::registerType<WorldContext>(QQmlEngine *qml, QJSEngine *script)
{
    qmlRegisterType<WorldContext>("OpenSR.World", 1, 0, "WorldContext");
}

template <> WorldContext *WorldObject::createObject(WorldObject *parent, quint32 id)
{
    return new WorldContext(parent, id);
}

template <> quint32 WorldObject::staticTypeId<WorldContext>()
{
    static const quint32 id = typeIdFromClassName(WorldContext::staticMetaObject.className());
    return id;
}

template <> const QMetaObject *WorldObject::staticTypeMeta<WorldContext>()
{
    return &WorldContext::staticMetaObject;
}

WorldContext::WorldContext(WorldObject *parent, quint32 id)
    : WorldObject(parent, id), m_resources(new ResourceManager(this))
{
}

WorldContext::~WorldContext()
{
}

quint32 WorldContext::typeId() const
{
    return staticTypeId<WorldContext>();
}

QString WorldContext::namePrefix() const
{
    return tr("World");
}

PlanetarySystem *WorldContext::currentSystem() const
{
    return m_currentSystem;
}

void WorldContext::setCurrentSystem(PlanetarySystem *system)
{
    if (m_currentSystem != system)
    {
        m_currentSystem = system;
        emit(currentSystemChanged());
    }
}

bool WorldContext::save(QDataStream &stream) const
{
    quint32 id = 0;

    if (m_currentSystem)
    {
        id = m_currentSystem->id();
    }

    stream << id << m_resources->id();
    return stream.status() == QDataStream::Ok;
}

bool WorldContext::load(QDataStream &stream, const QMap<quint32, WorldObject *> &objects)
{
    quint32 id{};
    stream >> id;
    auto it = objects.find(id);
    if (it != objects.end())
    {
        setCurrentSystem(qobject_cast<PlanetarySystem *>(it.value()));
    }

    stream >> id;
    it = objects.find(id);
    if (it != objects.end())
    {
        ResourceManager *m = qobject_cast<ResourceManager *>(it.value());
        if (m)
        {
            delete m_resources;
            m_resources = m;
            emit(resourcesChanged());
        }
    }

    return stream.status() == QDataStream::Ok;
}

WorldObject *WorldContext::playerShip() const
{
    return m_playerShip;
}

void WorldContext::setPlayerShip(WorldObject *ship)
{
    if (m_playerShip == ship)
    {
        return;
    }

    Ship *oldPlayerShip = qobject_cast<Ship *>(m_playerShip);
    if (oldPlayerShip)
    {
        disconnect(oldPlayerShip, &Ship::shipArrived, this, &WorldContext::onShipArrived);
    }

    m_playerShip = ship;
    emit playerShipChanged(ship);

    Ship *newPlayerShip = qobject_cast<Ship *>(m_playerShip);
    if (newPlayerShip)
    {
        connect(newPlayerShip, &Ship::shipArrived, this, &WorldContext::onShipArrived);
    }
}

bool WorldContext::checkPlannedActions() const
{
    if (m_playerShip)
    {
        return qobject_cast<Ship *>(m_playerShip)->checkPlannedActions();
    }

    return false;
}

ResourceManager *WorldContext::resources() const
{
    return m_resources;
}

QObject *WorldContext::findObject(const QString &name) const
{
    return findChild<QObject *>(name);
}

void WorldContext::onShipArrived()
{
    m_shipIsMoving = false;
    emit plannedActionsCompleted();
}

WorldObject *WorldContext::planetToEnter() const
{
    return m_planetToEnter;
}

void WorldContext::setPlanetToEnter(WorldObject *planet)
{
    if (m_planetToEnter == planet)
    {
        return;
    }
    m_planetToEnter = planet;
    emit planetToEnterChanged(planet);
}

QPointF WorldContext::movementPosition()
{
    return m_planetPosition;
}

void WorldContext::setMovementPosition(const QPointF &pos)
{
    if (m_planetPosition == pos)
    {
        return;
    }
    m_planetPosition = pos;
    emit movementPositionChanged(pos);
}

void WorldContext::setObjectToShoot(WorldObject *obj)
{
    qDebug() << "setObjectToShoot(WorldObject *obj)";
    if(m_objectToShoot == obj) 
        return;
    m_objectToShoot = obj;
    emit objectToShootChanged(obj);
}

void WorldContext::prepareToShoot(WorldObject* obj)
{
    if(obj)
        setObjectToShoot(obj);
}


void WorldContext::damageObject() 
{
    if(auto* allowedChild = qobject_cast<Asteroid*>(objectToShoot()))
        allowedChild->damageObject();
    setObjectToShoot(nullptr);

}

bool WorldContext::isChoosingToShoot() const
{
    return m_isChoosingToShoot;
}

void WorldContext::setIsChoosingToShoot(bool isChoosingToShoot)
{
    if(m_isChoosingToShoot == isChoosingToShoot) return;
    m_isChoosingToShoot = isChoosingToShoot;
    emit isChoosingToShootChanged(isChoosingToShoot);
}

WorldObject* WorldContext::objectToShoot() const
{
    return m_objectToShoot;
}

} // namespace World
} // namespace OpenSR
