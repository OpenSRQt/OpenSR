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

#include "World.h"
#include "MannedObject.h"
#include "Resource.h"

namespace OpenSR
{
namespace World
{

class OPENSR_WORLD_API ShipStyle: public Resource
{
    Q_GADGET
    Q_PROPERTY(QString texture READ texture  WRITE setTexture)
    Q_PROPERTY(int     width   READ width    WRITE setWidth)

public:
    struct Data {
        QString texture;
        int width;
    };

    int width() const;
    void setWidth(int);
    QString texture() const;
    void setTexture(const QString& texture);
};

bool operator==(const ShipStyle& one, const ShipStyle& another);

QDataStream& operator<<(QDataStream & stream, const ShipStyle& style);
QDataStream& operator>>(QDataStream & stream, ShipStyle& style);
QDataStream& operator<<(QDataStream & stream, const ShipStyle::Data& data);
QDataStream& operator>>(QDataStream & stream, ShipStyle::Data& data);

class OPENSR_WORLD_API Ship: public MannedObject
{
    Q_OBJECT
    OPENSR_WORLD_OBJECT

    Q_PROPERTY(ShipAffiliation affiliation READ affiliation WRITE setAffiliation NOTIFY affiliationChanged)
    Q_PROPERTY(ShipRank        rank        READ rank        WRITE setRank        NOTIFY rankChanged)
    Q_PROPERTY(float period READ period WRITE setPeriod NOTIFY periodChanged)
    Q_PROPERTY(float time READ time WRITE setTime NOTIFY timeChanged)
    Q_PROPERTY(float speed READ speed NOTIFY speedChanged STORED false)
    Q_PROPERTY(QPointF destination READ destination WRITE setDestination NOTIFY destinationChanged)

public:
    enum class ShipAffiliation {
        Unspecified = 0,
        Gaal, Fei, People, Peleng, Malok,
        Keller, Terron, Blazer,
        UnknownRace
    };
    Q_ENUM(ShipAffiliation);

    enum class ShipRank {
        Unspecified = 0,
        Diplomat, Liner, Ranger, Pirate, Warrior, Transport
    };
    Q_ENUM(ShipRank);

    Q_INVOKABLE Ship(WorldObject *parent = 0, quint32 id = 0);
    virtual ~Ship();

    float period() const;
    float time() const;
    float speed() const;
    QPointF destination() const;


    virtual quint32 typeId() const;
    virtual QString namePrefix() const;
    ShipAffiliation affiliation() const;

    ShipRank rank() const;

    virtual void startTurn();
    virtual void processTurn(float time);
    virtual void finishTurn();

    Q_INVOKABLE void evalTrajectoryTo(const QPointF &dest);

public slots:
    void setAffiliation(ShipAffiliation affiliation);
    void setRank(ShipRank rank);
    void setPeriod(float period);
    void setTime(float time);
    void setDestination(QPointF destination);

signals:
    void affiliationChanged(ShipAffiliation affiliation);
    void rankChanged(ShipRank rank);
    void periodChanged();
    void timeChanged();
    void speedChanged();
    void destinationChanged();

private:
    void calcPosition(float dt = 0.0f);
    void calcSpeed();

    ShipAffiliation m_affiliation;
    ShipRank m_rank;
    float m_period;
    float m_time;
    float m_speed;
    QPointF m_destination;
};
}
}

Q_DECLARE_METATYPE(OpenSR::World::ShipStyle)
Q_DECLARE_METATYPE(OpenSR::World::ShipStyle::Data)

#endif // OPENSR_WORLD_SHIP_H
