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
#include "Ship.h"
#include <QDataStream>
#include <QMap>
#include <QtQml>

namespace OpenSR
{
namespace World
{
const quint32 WorldContext::m_staticTypeId = typeIdFromClassName(WorldContext::staticMetaObject.className());

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
    return WorldContext::m_staticTypeId;
}

template <> const QMetaObject *WorldObject::staticTypeMeta<WorldContext>()
{
    return &WorldContext::staticMetaObject;
}

WorldContext::WorldContext(WorldObject *parent, quint32 id) : WorldObject(parent, id), m_currentSystem(0)
{
    m_resources = new ResourceManager(this);
}

WorldContext::~WorldContext()
{
}

quint32 WorldContext::typeId() const
{
    return WorldContext::m_staticTypeId;
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
        id = m_currentSystem->id();

    stream << id << m_resources->id();
    return stream.status() == QDataStream::Ok;
}

bool WorldContext::load(QDataStream &stream, const QMap<quint32, WorldObject *> &objects)
{
    quint32 id;
    stream >> id;
    auto it = objects.find(id);
    if (it != objects.end())
        setCurrentSystem(qobject_cast<PlanetarySystem *>(it.value()));

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
        return;

    if (m_playerShip)
        disconnect(qobject_cast<Ship *>(m_playerShip), &Ship::shipArrived, this, &WorldContext::onShipArrived);

    m_playerShip = ship;
    emit playerShipChanged(ship);

    if (m_playerShip)
        connect(qobject_cast<Ship *>(m_playerShip), &Ship::shipArrived, this, &WorldContext::onShipArrived);
}

bool WorldContext::checkPlannedActions() const
{
    if (m_playerShip)
        return qobject_cast<Ship *>(m_playerShip)->checkPlannedActions();

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

WorldObject* WorldContext::planetToEnter() const
{
    return m_planetToEnter;
}

void WorldContext::setPlanetToEnter(WorldObject * planet)
{
    if(m_planetToEnter == planet) 
        return;
    m_planetToEnter = planet;
    emit planetToEnterChanged(planet);
}

QPointF WorldContext::movementPosition() 
{
    return m_planetPosition;
}

void WorldContext::setMovementPosition(const QPointF& pos) 
{
    if(m_planetPosition == pos) 
        return;
    m_planetPosition = pos;
    emit movementPositionChanged(pos);
}

WorldObject* WorldContext::objectToShoot() const
{
    return m_objectToShoot;
}

void WorldContext::setObjectToShoot(WorldObject *obj)
{
    if(m_objectToShoot == obj) 
        return;
    m_objectToShoot = obj;
    emit objectToShootChanged(obj);
}

void WorldContext::prepareToShoot(WorldObject* obj)
{
    if(obj)
        setObjectToShoot(obj);
    if(playerShip())
        qobject_cast<Ship*>(playerShip())->setActiveWeapon(nullptr);
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

bool WorldContext::setActiveWeapon(int pos) const
{
    if(!playerShip()) return false;
    auto weapon = m_container->getWeaponByPos(pos);
    auto ship = qobject_cast<Ship*>(playerShip());
    if(m_container && ship && weapon && (weapon != ship->activeWeapon()))
    {
        ship->setActiveWeapon(weapon);
        return true;
    }
    ship->setActiveWeapon(nullptr);
    return false;
}

Container* WorldContext::container() const {
    return m_container;
}

void WorldContext::setContainer(Container* i) {
    if(i == m_container) return;
    m_container = i;
}


}
}
