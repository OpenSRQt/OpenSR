import QtQuick 2.5
import OpenSR 1.0
import OpenSR.World 1.0

Item {
    id: self
    property WorldObject object
    property bool positioning: false
    property int mouseDelta: 0

    property real shipAngle: object && object.hasOwnProperty("angle") ? object.angle : 0

    signal entered(WorldObject obj)
    signal exited(WorldObject obj)

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
            } else if (WorldManager.typeName(object.typeId) === "OpenSR::World::DesertPlanet" || WorldManager.typeName(object.typeId) === "OpenSR::World::InhabitedPlanet") {
                item.planet = object;
            } else if (WorldManager.typeName(object.typeId) === "OpenSR::World::Ship") {
                item.source = object.style.texture;
                item.height = item.width = object.style.width;
                item.ship = object;
            } else if (WorldManager.typeName(object.typeId) === "OpenSR::World::SpaceStation") {
                item.source = object.style.texture;
            }
        }
        SpaceMouseArea {
            id: mouse
            anchors.fill: parent
            radius: Math.min(width / 2, height / 2) - mouseDelta
            onEntered: mouseEntered()
            onExited: mouseExited()
        }
    }

    Component {
        id: defaultComponent
        AnimatedImage {
            cache: false
            MouseArea {
                id: item // ?
                anchors.fill: parent
                propagateComposedEvents: true
            }
        }
    }

    Component {
        id: planetComponent
        PlanetItem {
            id: planetItem
            property bool isWaitingForShipArrival: false

            MouseArea {
                propagateComposedEvents: true
                anchors.fill: parent
                onDoubleClicked: (mouse) => {
                    mouse.accepted = false;
                    if (!context.playerShip.isMoving && context.planetToEnter == null) {
                        context.planetToEnter = planetItem.planet;
                        isWaitingForShipArrival = true;
                    }
                }
            }

            Connections {
                target: context

                function onPlannedActionsCompleted() {
                    if (planetItem.isWaitingForShipArrival) {
                        changeScreen("qrc:/OpenSR/PlanetView.qml", {
                            "planet": World.context.planetToEnter
                        });
                        planetItem.isWaitingForShipArrival = false;
                        context.planetToEnter = null;
                    }
                }
            }
        }
    }

    Component {
        id: shipComponent

        AnimatedImage {
            id: shipImage;
            cache: false
            property Ship ship
            opacity: 1
            scale: 1
            Behavior on opacity {
                NumberAnimation {
                    duration: 500
                    easing.type: Easing.InOutQuad
                }
            }
            Behavior on scale {
                NumberAnimation { duration: 2000 }
            }
            Connections {
                target: ship

                function onEnterPlace() {
                    shipImage.opacity = 0;
                    shipImage.scale = 0.5;
                }

                function onExitPlace() {
                    shipImage.opacity = 1;
                    shipImage.scale = 1;
                }
            }
        }

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
        } else if (WorldManager.typeName(object.typeId) === "OpenSR::World::DesertPlanet" || WorldManager.typeName(object.typeId) === "OpenSR::World::InhabitedPlanet") {
            objectLoader.sourceComponent = planetComponent;
            positioning = true;
        } else if (WorldManager.typeName(object.typeId) === "OpenSR::World::SpaceStation") {
            objectLoader.sourceComponent = defaultComponent;
            positioning = true;
        } else if (WorldManager.typeName(object.typeId) === "OpenSR::World::Ship") {
            objectLoader.sourceComponent = shipComponent;
            positioning = true;
        }
    }

    function mouseEntered() {
        if (object) {
            entered(object);
        }
    }

    function mouseExited() {
        exited(object);
    }

    function radToDeg(rad) {
        return rad * (180 / Math.PI) + 90;
    }
}