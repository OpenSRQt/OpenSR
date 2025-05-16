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

#ifndef OPENSR_WORLD_SHIP_H
#define OPENSR_WORLD_SHIP_H

#include "MannedObject.h"
#include "Resource.h"
#include "World.h"
#include <QPoint>

namespace OpenSR
{
namespace World
{

class OPENSR_WORLD_API ShipStyle : public Resource
{
    Q_GADGET
    Q_PROPERTY(QString texture READ texture WRITE setTexture)
    Q_PROPERTY(int width READ width WRITE setWidth)

public:
    struct Data
    {
        QString texture;
        int width{};
    };

    int width() const;
    void setWidth(int);
    QString texture() const;
    void setTexture(const QString &texture);
};

bool operator==(const ShipStyle &one, const ShipStyle &another);

QDataStream &operator<<(QDataStream &stream, const ShipStyle &style);
QDataStream &operator>>(QDataStream &stream, ShipStyle &style);
QDataStream &operator<<(QDataStream &stream, const ShipStyle::Data &data);
QDataStream &operator>>(QDataStream &stream, ShipStyle::Data &data);

class OPENSR_WORLD_API Ship : public MannedObject
{
    Q_OBJECT
    OPENSR_WORLD_OBJECT

    Q_PROPERTY(ShipAffiliation affiliation READ affiliation WRITE setAffiliation NOTIFY affiliationChanged)
    Q_PROPERTY(ShipRank rank READ rank WRITE setRank NOTIFY rankChanged)
    Q_PROPERTY(float angle READ angle WRITE setAngle NOTIFY angleChanged)
    Q_PROPERTY(float speed READ speed NOTIFY speedChanged STORED false)
    Q_PROPERTY(QPointF destination READ destination WRITE setDestination NOTIFY destinationChanged)
    Q_PROPERTY(bool isMoving READ isMoving WRITE setIsMoving NOTIFY isMovingChanged)

public:
    enum class ShipAffiliation
    {
        Unspecified = 0,
        Gaal,
        Fei,
        People,
        Peleng,
        Malok,
        Keller,
        Terron,
        Blazer,
        UnknownRace
    };
    Q_ENUM(ShipAffiliation)

    enum class ShipRank
    {
        Unspecified = 0,
        Diplomat,
        Liner,
        Ranger,
        Pirate,
        Warrior,
        Transport
    };
    Q_ENUM(ShipRank)

    Q_INVOKABLE Ship(WorldObject *parent = 0, quint32 id = 0);
    ~Ship() override;

    quint32 typeId() const override;
    QString namePrefix() const override;

    ShipAffiliation affiliation() const;
    ShipRank rank() const;

    float angle() const;
    float speed() const;
    QPointF destination() const;
    bool isMoving() const;

    Q_INVOKABLE void exitThePlace();
    void setAffiliation(ShipAffiliation affiliation);
    void setRank(ShipRank rank);
    void setDestination(QPointF destination);
    void setAngle(float angle);
    Q_INVOKABLE bool checkProximity(QPointF center, WorldObject *obj, int radius);
    void checkPlanetProximity(WorldObject* planetToEnter);
    void setIsMoving(bool isMoving);

    static const float normalLinearSpeed;
    static const float normalAngularSpeed;

    void startTurn() override;
    void processTurn(float time) override;
    void finishTurn() override;
    Q_INVOKABLE void prepareToMove(const QPointF &dest);
    void calcTrajectory(const QPointF &destination);
    bool checkPlannedActions() const;

signals:
    void affiliationChanged(ShipAffiliation affiliation);
    void rankChanged(ShipRank rank);
    void timeChanged();
    void speedChanged();
    void destinationChanged();
    void angleChanged();
    void isMovingChanged();
    void shipArrived();

    void enterPlace();
    void exitPlace();

private:
    QPointF calcPosition(const float dt, const float angle, const QPointF &pos, const QPointF &dest);
    float calcAngle(const float dt, const float angle, const QPointF &pos, const QPointF &dest);
    void updatePosition(const float dt = 0.0f);
    void updateAngle(const float dt = 0.0f);
    void normalizeAnlge(float &deltaAngle);
    void initTargetAngle(const QPointF &pos, const QPointF &dest);
    void correctLinearSpeed(const QPointF &dest, const QPointF &pos);
    void resetSpeedParams();

    ShipAffiliation m_affiliation{};
    ShipRank m_rank{};

    float m_angle{};
    float m_speed{};
    float m_angularSpeed{};
    float m_targetAngle{};
    QPointF m_destination{};
    QPointF m_start_position{};

    bool m_isNearPlanet = false;
    bool m_isMoving = false;
    bool m_actionsPlanned = false;
};
} // namespace World
} // namespace OpenSR

Q_DECLARE_METATYPE(OpenSR::World::ShipStyle)
Q_DECLARE_METATYPE(OpenSR::World::ShipStyle::Data)

#endif // OPENSR_WORLD_SHIP_H