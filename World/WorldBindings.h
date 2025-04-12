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

#ifndef OPENSR_WORLD_WORLDBINDINGS_H
#define OPENSR_WORLD_WORLDBINDINGS_H

#include "WorldObject.h"

#include <QJSEngine>
#include <QMetaObject>
#include <QQmlEngine>

#include <QDebug>

class QQmlEngine;

#define WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(Factory, Class)\
    OpenSR::World::Class* Factory::Class(WorldObject *parent) {\
        OpenSR::World::Class *obj = WorldObject::createObject<World::Class>(parent);\
        QQmlEngine::setObjectOwnership((QObject*)obj, QQmlEngine::JavaScriptOwnership);\
        return obj;\
    }

#define WORLD_JS_DEFAULT_GADGET_CONSTRUCTOR(Factory, Class)\
    Class Factory::Class() {\
        return World::Class();\
    }

namespace OpenSR
{
namespace World
{
void bindWorldTypes(QJSEngine *script, QQmlEngine *qml);

template<class T>
static void bindEnumsToJS(QJSEngine *script)
{
    QJSValue world = script->globalObject().property("World");

    if (world.isUndefined())
        return;
    
    const QMetaObject *obj = &T::staticMetaObject;

    for (int i = 0; i < obj->enumeratorCount(); ++i) {
        QMetaEnum metaEnum = obj->enumerator(i);
        QJSValue enumObj = script->newObject();

        for (int j = 0; j < metaEnum.keyCount(); ++j) {
            enumObj.setProperty(metaEnum.key(j), metaEnum.value(j));
        }

        world.setProperty(metaEnum.name(), enumObj);
    }
}

}
}

#endif // OPENSR_WORLD_WORLDBINDINGS_H
