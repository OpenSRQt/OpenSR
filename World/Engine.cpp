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

#include "Engine.h"

#include <QtQml>

namespace OpenSR
{
namespace World
{
template <> void WorldObject::registerType<Engine>(QQmlEngine *qml, QJSEngine *script)
{
    qmlRegisterType<Engine>("OpenSR.World", 1, 0, "Engine");
}

template <> Engine *WorldObject::createObject(WorldObject *parent, quint32 id)
{
    return new Engine(parent, id);
}

template <> quint32 WorldObject::staticTypeId<Engine>()
{
    static const quint32 id = typeIdFromClassName(Engine::staticMetaObject.className());
    return id;
}

template <> const QMetaObject *WorldObject::staticTypeMeta<Engine>()
{
    return &Engine::staticMetaObject;
}

Engine::Engine(WorldObject *parent, quint32 id) : Equipment(parent, id)
{
}

Engine::~Engine()
{
}

quint32 Engine::typeId() const
{
    return staticTypeId<Engine>();
}

QString Engine::namePrefix() const
{
    return tr("Engine");
}
} // namespace World
} // namespace OpenSR
