import QtQuick 2.3
import OpenSR 1.0
import OpenSR.World 1.0

Item {
    id: self
    property WorldObject object
    property bool positioning: false
    property int mouseDelta: 0

    property real shipAngle: object && object.hasOwnProperty("angle") ? object.angle : 0

    signal entered(WorldObject obj)
    signal exited

    x: positioning && object ? object.position.x : 0
    y: positioning && object ? object.position.y : 0
    rotation: positioning && object ? radToDeg(shipAngle) : 0

    Loader {
        id: objectLoader
        anchors.centerIn: parent
        asynchronous: false

        onLoaded: {
            if (WorldManager.typeName(object.typeId) === "OpenSR::World::PlanetarySystem") {
                item.source = object.style.star;
            } else if (WorldManager.typeName(object.typeId) === "OpenSR::World::Asteroid") {
                item.source = object.style.texture;
            } else if (WorldManager.typeName(object.typeId) === "OpenSR::World::InhabitedPlanet") {
                item.planet = object;
            } else if (WorldManager.typeName(object.typeId) === "OpenSR::World::Ship") {
                item.source = object.style.texture;
                item.height = item.width = object.style.width;
            } else if (WorldManager.typeName(object.typeId) === "OpenSR::World::SpaceStation") {
                item.source = object.style.texture;
            }
        }
        SpaceMouseArea {
            id: mouse
            anchors.fill: parent
            radius: Math.min(width / 2, height / 2) - mouseDelta
            onEntered: mouseEntered(object)
            onExited: mouseExited()
        }
    }

    Component {
        id: defaultComponent
        AnimatedImage {
            cache: false
        }
    }
    Component {
        id: planetComponent
        PlanetItem {}
    }

    onObjectChanged: {
        objectLoader.source = "";
        objectLoader.sourceComponent = undefined;

        if (!object) {
            positioning = false;
            rotation = 0;
            return;
        }

        if (WorldManager.typeName(object.typeId) === "OpenSR::World::PlanetarySystem") {
            objectLoader.sourceComponent = defaultComponent;
            positioning = false;
        } else if (WorldManager.typeName(object.typeId) === "OpenSR::World::Asteroid") {
            objectLoader.sourceComponent = defaultComponent;
            positioning = true;
        } else if (WorldManager.typeName(object.typeId) === "OpenSR::World::InhabitedPlanet") {
            objectLoader.sourceComponent = planetComponent;
            positioning = true;
        } else if (WorldManager.typeName(object.typeId) === "OpenSR::World::SpaceStation") {
            objectLoader.sourceComponent = defaultComponent;
            positioning = true;
        } else if (WorldManager.typeName(object.typeId) === "OpenSR::World::Ship") {
            objectLoader.sourceComponent = defaultComponent;
            positioning = true;
        }
    }

    // onShipAngleChanged: {
    //     if (object && WorldManager.typeName(object.typeId) === "OpenSR::World::Ship") {
    //         rotation = radToDeg(shipAngle);
    //     }
    // }

    function mouseEntered() {
        if (object)
            entered(object);
    }

    function mouseExited() {
        exited(object);
    }

    function radToDeg(rad) {
        return rad * (180 / Math.PI) + 90;
    }
}
