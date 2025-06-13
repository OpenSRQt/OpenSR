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

#include "Weapon.h"
#include <QtQml>

namespace OpenSR
{
namespace World
{

QString WeaponStyle::SoundPath() const
{
    return getData<Data>().SoundPath;
}
QString WeaponStyle::preview() const
{
    return getData<Data>().preview;
}
QString WeaponStyle::weaponAnim() const
{
    return getData<Data>().weaponAnim;
}
QString WeaponStyle::typeWeapon() const
{
    return getData<Data>().typeWeapon;
}
int WeaponStyle::radius() const
{
    return getData<Data>().radius;
}
int WeaponStyle::hitPoints() const
{
    return getData<Data>().hitPoints;
}

void WeaponStyle::setSoundPath(const QString &c)
{
    auto d = getData<Data>();
    d.SoundPath = c;
    setData(d);
}
void WeaponStyle::setPreview(const QString &c)
{
    auto d = getData<Data>();
    d.preview = c;
    setData(d);
}
void WeaponStyle::setWeaponAnim(const QString &c)
{
    auto d = getData<Data>();
    d.weaponAnim = c;
    setData(d);
}
void WeaponStyle::setTypeWeapon(const QString &c)
{
    auto d = getData<Data>();
    d.typeWeapon = c;
    setData(d);
}
void WeaponStyle::setRadius(int c)
{
    auto d = getData<Data>();
    d.radius = c;
    setData(d);
}
void WeaponStyle::setHitPoints(int c)
{
    auto d = getData<Data>();
    d.hitPoints = c;
    setData(d);
}

bool operator==(const WeaponStyle &one, const WeaponStyle &another)
{
    return (one.SoundPath() == another.SoundPath()) && (one.preview() == another.preview()) &&
           (one.weaponAnim() == another.weaponAnim()) && (one.typeWeapon() == another.typeWeapon()) &&
           (one.radius() == another.radius()) && (one.hitPoints() == another.hitPoints());
}

QDataStream &operator<<(QDataStream &stream, const WeaponStyle &style)
{
    return stream << style.id();
}

QDataStream &operator>>(QDataStream &stream, WeaponStyle &style)
{
    quint32 id = 0;
    stream >> id;
    ResourceManager *m = ResourceManager::instance();
    Q_ASSERT(m != 0);
    Resource::replaceData(style, m->getResource(id));
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const WeaponStyle::Data &data)
{
    return stream << data.SoundPath << data.preview << data.weaponAnim << data.typeWeapon << data.radius
                  << data.hitPoints;
}

QDataStream &operator>>(QDataStream &stream, WeaponStyle::Data &data)
{
    return stream >> data.SoundPath >> data.preview >> data.weaponAnim >> data.typeWeapon >> data.radius >>
           data.hitPoints;
}

template <> void WorldObject::registerType<Weapon>(QQmlEngine *qml, QJSEngine *script)
{
    qRegisterMetaType<WeaponStyle>();
    qRegisterMetaType<WeaponStyle::Data>();
    qmlRegisterType<Weapon>("OpenSR.World", 1, 0, "Weapon");
}

template <> Weapon *WorldObject::createObject(WorldObject *parent, quint32 id)
{
    return new Weapon(parent, id);
}

template <> quint32 WorldObject::staticTypeId<Weapon>()
{
    static const quint32 id = typeIdFromClassName(Weapon::staticMetaObject.className());
    return id;
}

template <> const QMetaObject *WorldObject::staticTypeMeta<Weapon>()
{
    return &Weapon::staticMetaObject;
}

Weapon::Weapon(WorldObject *parent, quint32 id) : Equipment(parent, id)
{
}

Weapon::~Weapon()
{
}

quint32 Weapon::typeId() const
{
    return staticTypeId<Weapon>();
}

QString Weapon::namePrefix() const
{
    return tr("Weapon");
}

WeaponStyle Weapon::style() const
{
    return m_style;
}
void Weapon::setStyle(const WeaponStyle &style)
{
    if (m_style == style)
    {
        return;
    }
    m_style = style;
    emit styleChanged(style);
}

int Weapon::hitPoints() const
{
    return style().hitPoints();
}

} // namespace World
} // namespace OpenSR
