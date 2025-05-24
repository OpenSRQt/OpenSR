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

#ifndef OPENSR_WORLD_CONTAINER_H
#define OPENSR_WORLD_CONTAINER_H

#include "WorldObject.h"
#include "SpaceObject.h"
#include "World.h"
#include "Weapon.h"
#include "Item.h"

namespace OpenSR
{
namespace World
{
class OPENSR_WORLD_API Container : public SpaceObject
{
    Q_OBJECT
    OPENSR_WORLD_OBJECT

public:
    Q_INVOKABLE Container(WorldObject *parent = 0, quint32 id = 0);
    ~Container() override;

    quint32 typeId() const override;
    QString namePrefix() const override;

    Q_INVOKABLE void addWeapon(Weapon* weapon, int pos);
    Q_INVOKABLE void addResources(Item* item);
    Q_INVOKABLE void removeWeapon(int pos);
    Q_INVOKABLE void removeResources(Item* item);
    Q_INVOKABLE Weapon* getWeaponByPos(int pos) const;
    Q_INVOKABLE QMap<Item*,int> getResources() const;
    Q_INVOKABLE int getResources(Item* item) const;


    friend QDataStream &operator<<(QDataStream &out, const OpenSR::World::Container* container);
    friend QDataStream &operator>>(QDataStream &in, OpenSR::World::Container* &container);
private:
    int weight = 0;
    int maxWeight = 1000;
    QMap<int,Weapon*> weapons;
    QMap<Item*,int> resources;

};
} // namespace World
} // namespace OpenSR

#endif // OPENSR_WORLD_CONTAINER_H
