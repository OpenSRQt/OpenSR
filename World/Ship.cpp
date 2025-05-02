/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2:
   Dominators" Copyright (C) 2015 Kosyak <ObKo@mail.ru>

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

#include <qpoint.h>

#include <QtMath>
#include <QtQml/QQmlEngine>
#include <cmath>

#include "WorldBindings.h"

#define CONST_SPEED 0.2f
#define NORMAL_ANGULAR_SPEED 0.003f

namespace OpenSR {
namespace World {
const quint32 Ship::m_staticTypeId =
    typeIdFromClassName(Ship::staticMetaObject.className());

template <>
void WorldObject::registerType<Ship>(QQmlEngine *qml, QJSEngine *script) {
    qRegisterMetaType<ShipStyle>();
    qRegisterMetaTypeStreamOperators<ShipStyle>();
    qRegisterMetaType<ShipStyle::Data>();
    qRegisterMetaTypeStreamOperators<ShipStyle::Data>();
    bindEnumsToJS<Ship>(script);
    qmlRegisterType<Ship>("OpenSR.World", 1, 0, "Ship");
}

template <>
Ship *WorldObject::createObject(WorldObject *parent, quint32 id) {
    return new Ship(parent, id);
}

template <>
quint32 WorldObject::staticTypeId<Ship>() {
    return Ship::m_staticTypeId;
}

template <>
const QMetaObject *WorldObject::staticTypeMeta<Ship>() {
    return &Ship::staticMetaObject;
}

/**************************************************************************************************/

int ShipStyle::width() const { return getData<Data>().width; }

void ShipStyle::setWidth(int w) {
    auto d = getData<Data>();
    d.width = w;
    setData(d);
}

QString ShipStyle::texture() const { return getData<Data>().texture; }

void ShipStyle::setTexture(const QString &texture) {
    auto d = getData<Data>();
    d.texture = texture;
    setData(d);
}

bool operator==(const ShipStyle &one, const ShipStyle &another) {
    return one.texture() == another.texture();
}

/*  Ship */
Ship::Ship(WorldObject *parent, quint32 id)
    : MannedObject(parent, id),
      m_speed(CONST_SPEED),
      m_angularSpeed(NORMAL_ANGULAR_SPEED) {}

Ship::~Ship() {}

quint32 Ship::typeId() const { return Ship::m_staticTypeId; }

QString Ship::namePrefix() const { return tr("Ship"); }

Ship::ShipAffiliation Ship::affiliation() const { return m_affiliation; }

Ship::ShipRank Ship::rank() const { return m_rank; }

float Ship::time() const { return m_time; }

float Ship::angularSpeed() const { return m_angularSpeed; }

float Ship::angle() const { return m_angle; }

float Ship::speed() const { return m_speed; }

QPointF Ship::destination() const { return m_destination; }

bool Ship::isMoving() const { return m_isMoving; }

void Ship::setAffiliation(Ship::ShipAffiliation affiliation) {
    if (m_affiliation == affiliation) return;

    m_affiliation = affiliation;
    emit affiliationChanged(m_affiliation);
}

void Ship::setRank(Ship::ShipRank rank) {
    if (m_rank == rank) return;

    m_rank = rank;
    emit rankChanged(m_rank);
}

void Ship::setTime(float time) {
    if (time != m_time) {
        m_time = time;
        emit(timeChanged());
    }
}

void Ship::setAngle(float angle) {
    if (angle != m_angle) {
        m_angle = angle;
        emit(angleChanged());
    }
}

void Ship::setDestination(QPointF destination) {
    if (destination != m_destination) {
        m_destination = destination;
        emit(destinationChanged());
    }
}

void Ship::setIsMoving(bool isMoving) {
    if (isMoving != m_isMoving) {
        m_isMoving = isMoving;
        emit(isMovingChanged());
    }
}

void Ship::normalizeAngle(float &deltaAngle) {
    while (deltaAngle > M_PI) deltaAngle -= 2 * M_PI;

    while (deltaAngle < -M_PI) deltaAngle += 2 * M_PI;
}

void Ship::initTargetAngle()
{
    QPointF directionalVector = m_destination - position();
    float directionNorm = std::sqrt(directionalVector.x() * directionalVector.x() + directionalVector.y() * directionalVector.y());
    directionalVector /= directionNorm;

    m_targetAngle = std::atan2(directionalVector.y(), directionalVector.x());
}

void Ship::correctLinearSpeed() 
{
    const float deltaX = m_destination.x() - position().x();
    const float deltaY = m_destination.y() - position().y();
    const float turnRadius = (deltaX * deltaX + deltaY * deltaY) / (2 * abs(deltaX * sin(m_angle) - deltaY * cos(m_angle)));
    if (turnRadius < m_speed / m_angularSpeed) 
    {
        m_speed = m_angularSpeed * turnRadius;
    }
}

void Ship::startMovement(QPointF destination) // TODO: replace QPointF with QVector2d
{  
    setIsMoving(true);
    m_isNearPlanet = false;
    m_start_position = position();
    setDestination(destination);
    correctLinearSpeed();
}

void Ship::processMovement(float time) {
    calcAngle(time);
    calcPosition(time);
}

void Ship::calcAngle(float dt) {
    initTargetAngle();
    float deltaAngle = m_targetAngle - m_angle;
    normalizeAngle(deltaAngle);

    if (std::abs(deltaAngle) <= dt * m_angularSpeed ||
        m_angle == m_targetAngle) {
        setAngle(m_targetAngle);
    } else {
        const float rotateDirection = deltaAngle >= 0 ? 1.0f : -1.0f;
        const float newAngle = m_angle + dt * m_angularSpeed * rotateDirection;
        setAngle(newAngle);
    }
}


void Ship::calcPosition(float dt)
{
    const QPointF direction = m_destination - position();
    const float distance = std::sqrt(direction.x() * direction.x() + direction.y() * direction.y());
    
    if (distance <= dt * m_speed || m_destination == m_start_position) 
    {
        setPosition(m_destination);
        m_speed = CONST_SPEED;
        setIsMoving(false);
        emit shipArrived();
    } 
    else 
    {
        const QPointF directionalVector(std::cos(m_angle), std::sin(m_angle));
        const QPointF next = position() + directionalVector * m_speed * dt;
        setPosition(next);
    }
}

void Ship::evalTrajectoryTo(const QPointF &dest) {
    qDebug() << Q_FUNC_INFO;
    auto startPos = this->position();

    qDebug() << "from" << startPos << "to" << dest;

    auto dx = dest.x() - startPos.x();
    auto dy = dest.y() - startPos.y();
    qDebug() << startPos << dest;
    qDebug() << QString("dx = %1, dy = %2").arg(dx).arg(dy);

    const int h = 20.0;
    const qreal avgSq = qSqrt(dx * dx + dy * dy);
    QList<BezierCurve> traj;
    if (dx > dy) {
        qreal alphaTan = dy / dx;
        auto dxStep = h * dx / avgSq;
        int fullSteps = static_cast<int>(dx / dxStep);
        for (int i = 1; i <= fullSteps; ++i) {
            const qreal xx = dxStep * static_cast<qreal>(i);
            const qreal yy = xx * alphaTan;
            auto p = QPointF(xx, yy);
            auto curve = BezierCurve();
            curve.p0 = curve.p1 = curve.p2 = curve.p3 = startPos + p;
            traj.append(curve);
        }
    } else {
        qreal alphaTan = dx / dy;
        auto dyStep = h * dy / avgSq;
        int fullSteps = static_cast<int>(dy / dyStep);
        for (int i = 1; i <= fullSteps; ++i) {
            const qreal yy = dyStep * static_cast<qreal>(i);
            const qreal xx = yy * alphaTan;
            auto p = QPointF(xx, yy);
            auto curve = BezierCurve();
            curve.p0 = curve.p1 = curve.p2 = curve.p3 = startPos + p;
            traj.append(curve);
        }
    }

    setTrajectory(traj);
    qDebug() << "new trajectory length = " << traj.size();
}

Q_INVOKABLE void Ship::exitThePlace() { emit exitPlace(); }

void Ship::checkPlanetProximity(WorldObject *planetToEnter,
                                const QPointF &shipPosition) {
    if (!planetToEnter) {
        return;
    }
    InhabitedPlanet *planet = qobject_cast<InhabitedPlanet *>(planetToEnter);
    int planetRadius = planet->style().radius();
    QPointF planetCenter = planet->position();

    const qreal distance = QLineF(shipPosition, planetCenter).length();

    if (distance <= planetRadius && !m_isNearPlanet) {
        m_isNearPlanet = true;
        emit enterPlace();
    }
}

}  // namespace World
}  // namespace OpenSR