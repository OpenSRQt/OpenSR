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

#ifndef OPENSR_WORLD_WORLDCONTEXT_H
#define OPENSR_WORLD_WORLDCONTEXT_H

#include "WorldObject.h"
#include "Container.h"
#include <QPoint>
#include "PlanetarySystem.h"
#include "ResourceManager.h"
#include "WorldObject.h"
#include <QPoint>
namespace OpenSR
{
namespace World
{
class OPENSR_WORLD_API WorldContext : public WorldObject
{
    Q_OBJECT
    OPENSR_WORLD_OBJECT

    Q_PROPERTY(PlanetarySystem *currentSystem READ currentSystem WRITE setCurrentSystem NOTIFY currentSystemChanged
                   STORED false)
    Q_PROPERTY(ResourceManager *resources READ resources NOTIFY resourcesChanged STORED false)
    Q_PROPERTY(WorldObject *playerShip READ playerShip NOTIFY playerShipChanged STORED false WRITE setPlayerShip)
    Q_PROPERTY(
        WorldObject *planetToEnter READ planetToEnter NOTIFY planetToEnterChanged STORED false WRITE setPlanetToEnter)
    Q_PROPERTY(WorldObject*  objectToShoot READ objectToShoot
        WRITE setObjectToShoot)    
    Q_PROPERTY(QPointF movementPosition READ movementPosition  
        NOTIFY movementPositionChanged STORED false WRITE setMovementPosition)
    Q_PROPERTY(bool isChoosingToShoot READ isChoosingToShoot WRITE setIsChoosingToShoot NOTIFY isChoosingToShootChanged)
    Q_PROPERTY(Container* container READ container WRITE setContainer)

public:
    Q_INVOKABLE WorldContext(WorldObject *parent = nullptr, quint32 id = 0);
    ~WorldContext() override;

    PlanetarySystem *currentSystem() const;
    ResourceManager *resources() const;

    Q_INVOKABLE QObject *findObject(const QString &name) const;

    void setCurrentSystem(PlanetarySystem *system);

    quint32 typeId() const override;
    QString namePrefix() const override;

    bool save(QDataStream &stream) const override;
    bool load(QDataStream &stream, const QMap<quint32, WorldObject *> &objects) override;

    WorldObject *playerShip() const;
    void setPlayerShip(WorldObject *);

    bool checkPlannedActions() const;

    WorldObject *planetToEnter() const;
    void setPlanetToEnter(WorldObject *);

    QPointF movementPosition();
    void setMovementPosition(const QPointF &pos);

    Q_INVOKABLE void prepareToShoot(WorldObject *);
    Q_INVOKABLE void damageObject();
    WorldObject* objectToShoot() const;
    void setObjectToShoot(WorldObject *);
    bool isChoosingToShoot() const;
    void setIsChoosingToShoot(bool value);

    Container* container() const;
    void setContainer(Container* i);
    Q_INVOKABLE bool setActiveWeapon(int pos) const;
    void resetActiveWeapon() const;
public slots:
    void onShipArrived();

signals:
    void currentSystemChanged();
    void resourcesChanged();

    void playerShipChanged(WorldObject *playerShip);
    void plannedActionsCompleted();

    void planetToEnterChanged(WorldObject *playerShip);
    void movementPositionChanged(const QPointF &pos);
    void enteringPlanet();
    void objectToShootChanged(WorldObject*);
    void isChoosingToShootChanged(bool isChoosingToShoot);

private:
    PlanetarySystem *m_currentSystem{};
    ResourceManager *m_resources{};
    WorldObject *m_playerShip{};
    WorldObject *m_planetToEnter{};
    WorldObject* m_objectToShoot{};
    QPointF m_planetPosition{};

    Container* m_container{};
    
    bool m_shipIsMoving{};
    bool m_isChoosingToShoot = false;
};
} // namespace World
} // namespace OpenSR

#endif // OPENSR_WORLD_WORLDCONTEXT_H
