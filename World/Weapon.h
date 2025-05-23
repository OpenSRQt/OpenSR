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

#ifndef OPENSR_WORLD_WEAPON_H
#define OPENSR_WORLD_WEAPON_H

#include "Equipment.h"
#include "World.h"
#include "Planet.h"
#include <QGraphicsItem>

namespace OpenSR
{
namespace World
{

class OPENSR_WORLD_API WeaponStyle : public Resource
{
    Q_GADGET

    Q_PROPERTY(QString SoundPath READ SoundPath WRITE setSoundPath)
    Q_PROPERTY(QString preview READ preview WRITE setPreview)
    Q_PROPERTY(QString weaponAnim READ weaponAnim WRITE setWeaponAnim)
    Q_PROPERTY(QString typeWeapon READ typeWeapon WRITE setTypeWeapon)
    Q_PROPERTY(int radius READ radius WRITE setRadius)
    Q_PROPERTY(int hitPoints READ hitPoints WRITE setHitPoints)

public:
    struct Data
    {
        QString SoundPath;
        QString preview;
        QString weaponAnim;
        QString typeWeapon;
        int radius;
        int hitPoints;
    };

    QString SoundPath() const;
    QString preview() const;
    QString weaponAnim() const;
    QString typeWeapon() const;
    int radius() const;
    int hitPoints() const;

    void setSoundPath(const QString &);
    void setPreview(const QString &);
    void setWeaponAnim(const QString &);
    void setTypeWeapon(const QString &);
    void setRadius(int);
    void setHitPoints(int);
};

bool operator==(const WeaponStyle &one, const WeaponStyle &another);

QDataStream &operator<<(QDataStream &stream, const WeaponStyle &style);
QDataStream &operator>>(QDataStream &stream, WeaponStyle &style);
QDataStream &operator<<(QDataStream &stream, const WeaponStyle::Data &data);
QDataStream &operator>>(QDataStream &stream, WeaponStyle::Data &data);

class OPENSR_WORLD_API Weapon: public Equipment
{
    Q_OBJECT
    OPENSR_WORLD_OBJECT

public:
    Q_INVOKABLE Weapon(WorldObject *parent = 0, quint32 id = 0);
    ~Weapon() override;

    quint32 typeId() const override;
    QString namePrefix() const override;
private:
    WeaponStyle style;

};
}
}

Q_DECLARE_METATYPE(OpenSR::World::WeaponStyle::Data)
Q_DECLARE_METATYPE(OpenSR::World::WeaponStyle)

#endif // OPENSR_WORLD_WEAPON_H
