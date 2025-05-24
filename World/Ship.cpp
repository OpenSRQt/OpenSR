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

#include "Ship.h"

#include "WorldBindings.h"

#include "Planet.h"
#include <QLine>
#include <QtMath>
#include <QtQml/QQmlEngine>
#include <cmath>

namespace OpenSR
{
namespace World
{
template <> void WorldObject::registerType<Ship>(QQmlEngine *qml, QJSEngine *script)
{
    qRegisterMetaType<ShipStyle>();
    qRegisterMetaType<ShipStyle::Data>();
    bindEnumsToJS<Ship>(script);
    qmlRegisterType<Ship>("OpenSR.World", 1, 0, "Ship");
}

template <> Ship *WorldObject::createObject(WorldObject *parent, quint32 id)
{
    return new Ship(parent, id);
}

template <> quint32 WorldObject::staticTypeId<Ship>()
{
    static const quint32 id = typeIdFromClassName(Ship::staticMetaObject.className());
    return id;
}

template <> const QMetaObject *WorldObject::staticTypeMeta<Ship>()
{
    return &Ship::staticMetaObject;
}

/**************************************************************************************************/

int ShipStyle::width() const
{
    return getData<Data>().width;
}

void ShipStyle::setWidth(int w)
{
    auto d = getData<Data>();
    d.width = w;
    setData(d);
}

QString ShipStyle::texture() const
{
    return getData<Data>().texture;
}

void ShipStyle::setTexture(const QString &texture)
{
    auto d = getData<Data>();
    d.texture = texture;
    setData(d);
}

bool operator==(const ShipStyle &one, const ShipStyle &another)
{
    return one.texture() == another.texture();
}

/*  Ship */

const float Ship::normalLinearSpeed = 0.3f;
const float Ship::normalAngularSpeed = 0.003f;

Ship::Ship(WorldObject *parent, quint32 id)
    : MannedObject(parent, id), m_speed(normalLinearSpeed), m_angularSpeed(normalAngularSpeed)
{
}

Ship::~Ship()
{
}

quint32 Ship::typeId() const
{
    return staticTypeId<Ship>();
}

QString Ship::namePrefix() const
{
    return tr("Ship");
}

Ship::ShipAffiliation Ship::affiliation() const
{
    return m_affiliation;
}

Ship::ShipRank Ship::rank() const
{
    return m_rank;
}

float Ship::angle() const
{
    return m_angle;
}

float Ship::speed() const
{
    return m_speed;
}

QPointF Ship::destination() const
{
    return m_destination;
}

bool Ship::isMoving() const
{
    return m_isMoving;
}

void Ship::setAffiliation(Ship::ShipAffiliation affiliation)
{
    if (m_affiliation == affiliation)
    {
        return;
    }

    m_affiliation = affiliation;
    emit affiliationChanged(m_affiliation);
}

void Ship::setRank(Ship::ShipRank rank)
{
    if (m_rank == rank)
    {
        return;
    }

    m_rank = rank;
    emit rankChanged(m_rank);
}

void Ship::setAngle(float angle)
{
    if (angle == m_angle)
    {
        return;
    }

    m_angle = angle;
    emit angleChanged();
}

void Ship::setDestination(QPointF destination)
{
    if (destination == m_destination)
    {
        return;
    }

    m_destination = destination;
    emit destinationChanged();
}

void Ship::setIsMoving(bool isMoving)
{
    if (isMoving == m_isMoving)
    {
        return;
    }

    m_isMoving = isMoving;
    emit isMovingChanged();
}

void Ship::normalizeAnlge(float &deltaAngle)
{
    while (deltaAngle > M_PI)
    {
        deltaAngle -= 2 * M_PI;
    }

    while (deltaAngle < -M_PI)
    {
        deltaAngle += 2 * M_PI;
    }
}

void Ship::resetSpeedParams()
{
    m_speed = normalLinearSpeed;
    m_angularSpeed = normalAngularSpeed;
}

void Ship::initTargetAngle(const QPointF &pos, const QPointF &dest)
{
    QPointF directionalVector = dest - pos;
    float directionNorm = static_cast<float>(
        std::sqrt(directionalVector.x() * directionalVector.x() + directionalVector.y() * directionalVector.y()));
    directionalVector /= directionNorm;

    m_targetAngle = static_cast<float>(std::atan2(directionalVector.y(), directionalVector.x()));
}

void Ship::correctLinearSpeed(const QPointF &dest, const QPointF &pos)
{
    const float deltaX = static_cast<float>(dest.x() - pos.x());
    const float deltaY = static_cast<float>(dest.y() - pos.y());
    const float turnRadius = static_cast<float>((deltaX * deltaX + deltaY * deltaY) /
                                                (2 * abs(deltaX * sin(m_angle) - deltaY * cos(m_angle))));

    if (turnRadius < m_speed / m_angularSpeed)
    {
        m_speed = m_angularSpeed * turnRadius;
    }
}

void Ship::prepareToMove(const QPointF &dest)
{
    if (dest != position())
    {
        m_isNearPlanet = false;
        m_actionsPlanned = true;
        m_start_position = position();
        setDestination(dest);
        calcTrajectory(dest);
    }
}

void Ship::startTurn()
{
    if (m_actionsPlanned)
    {
        setIsMoving(true);
        correctLinearSpeed(m_destination, m_start_position);
    }
    SpaceObject::startTurn();
}

void Ship::processTurn(const float time)
{
    if (m_actionsPlanned)
    {
        updateAngle(time);
        updatePosition(time);
    }
    SpaceObject::processTurn(time);
}

void Ship::finishTurn()
{
    m_actionsPlanned = false;
    SpaceObject::finishTurn();
}

float Ship::calcAngle(const float dt, const float angle, const QPointF &pos, const QPointF &dest)
{
    initTargetAngle(pos, dest);
    float deltaAngle = m_targetAngle - angle;
    normalizeAnlge(deltaAngle);

    if (std::abs(deltaAngle) <= dt * m_angularSpeed || angle == m_targetAngle)
    {
        return m_targetAngle;
    }
    else
    {
        const float rotateDirection = deltaAngle >= 0 ? 1.0f : -1.0f;
        const float newAngle = angle + dt * m_angularSpeed * rotateDirection;
        return newAngle;
    }
}

QPointF Ship::calcPosition(const float dt, const float angle, const QPointF &pos, const QPointF &dest)
{
    const float distance = static_cast<float>(QLineF(dest, pos).length());

    if (distance <= dt * m_speed || dest == m_start_position)
    {
        return dest;
    }
    else
    {
        const QPointF directionalVector(std::cos(angle), std::sin(angle));
        const QPointF newPosition = pos + directionalVector * m_speed * dt;
        return newPosition;
    }
}

void Ship::updatePosition(const float dt)
{
    const QPointF newPosition = calcPosition(dt, m_angle, position(), m_destination);

    if (newPosition == m_destination)
    {
        setIsMoving(false);
        setPosition(newPosition);
        resetSpeedParams();
        emit shipArrived();
    }
    else
    {
        setPosition(newPosition);
    }
}

void Ship::updateAngle(float dt)
{
    const float newAngle = calcAngle(dt, m_angle, position(), m_destination);
    setAngle(newAngle);
}

void Ship::calcTrajectory(const QPointF &dest)
{
    m_start_position = position();
    correctLinearSpeed(dest, m_start_position);

    const float pointDensityCoeff = 60.0f;
    const float pointStep = normalLinearSpeed * pointDensityCoeff / m_speed;

    float angle = m_angle;
    QPointF trajectoryPoint = m_start_position;

    QList<BezierCurve> trajectory;

    while (trajectoryPoint != dest)
    {
        const float distance = static_cast<float>(QLineF(dest, trajectoryPoint).length());

        if (distance < pointStep * m_speed)
        {
            break;
        }

        angle = calcAngle(pointStep, angle, trajectoryPoint, dest);
        trajectoryPoint = calcPosition(pointStep, angle, trajectoryPoint, dest);

        auto curve = BezierCurve();
        curve.p0 = curve.p1 = curve.p2 = curve.p3 = trajectoryPoint;
        trajectory.append(curve);
    }
    resetSpeedParams();

    setTrajectory(trajectory);
}

Q_INVOKABLE void Ship::exitThePlace()
{
    emit exitPlace();
}

// void Ship::checkPlanetProximity(WorldObject *planetToEnter)
// {
//     if (!planetToEnter)
//     {
//         return;
//     }
//     InhabitedPlanet *planet = qobject_cast<InhabitedPlanet *>(planetToEnter);
//     int planetRadius = planet->style().radius();
//     QPointF planetCenter = planet->position(); //
//     QPointF shipPosition = position(); //

//     const qreal distance = QLineF(shipPosition, planetCenter).length();

//     if (distance <= planetRadius && !m_isNearPlanet)
//     {
//         m_isNearPlanet = true;
//         emit(enterPlace());
//     }
// }

void Ship::checkPlanetProximity(WorldObject *planetToEnter)
{
    if (!planetToEnter)
    {
        return;
    }
    InhabitedPlanet *planet = qobject_cast<InhabitedPlanet *>(planetToEnter);
    int planetRadius = planet->style().radius();

    if (checkProximity(planet->position(), planetToEnter, planetRadius) && !m_isNearPlanet)
    {
        m_isNearPlanet = true;
        emit enterPlace();
    }
}

bool Ship::checkProximity(QPointF center, WorldObject *obj, int radius)
{
    if (!obj)
    {
        return false;
    }
    QPointF shipPosition = position();

    const qreal distance = QLineF(shipPosition, center).length();
    return distance <= radius;
}

bool Ship::checkPlannedActions() const
{
    return m_actionsPlanned;
}

int Ship::structure() const
{
    return m_structure;
}

void Ship::setStructure(int structure)
{
    if(structure == m_structure) return;
    structure = m_structure;
    emit structureChanged(structure);
}

} // namespace World
} // namespace OpenSR
