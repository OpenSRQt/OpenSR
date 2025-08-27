import QtQuick 2.5
import OpenSR 1.0
import OpenSR.World 1.0

Item {
    id: self
    property bool testConfig: isTestMode
    property WorldObject object
    property bool positioning: false
    property int mouseDelta: 0

    property real shipAngle: object && object.hasOwnProperty("angle") ? object.angle : 0

    signal entered(WorldObject obj)
    signal exited(WorldObject obj)

    x: positioning && object ? object.position.x : 0
    y: positioning && object ? object.position.y : 0
    rotation: positioning && object ? radToDeg(shipAngle) : 0

    function getComponentForObject(obj) {
        if (!obj)
            return defaultComponent;

        const typeName = WorldManager.typeName(obj.typeId);

        const componentMap = {
            "OpenSR::World::PlanetarySystem": defaultComponent,
            "OpenSR::World::Asteroid": defaultComponent,
            "OpenSR::World::DesertPlanet": planetComponent,
            "OpenSR::World::InhabitedPlanet": planetComponent,
            "OpenSR::World::SpaceStation": defaultComponent,
            "OpenSR::World::Ship": shipComponent
        };

        return componentMap[typeName] || defaultComponent;
    }

    Loader {
        id: objectLoader
        anchors.centerIn: parent
        asynchronous: false

        onLoaded: {
            if (!object)
                return;
            
            testBorder.border.color = ObjectConfig.getBorderColorForObject(object);

            if (testConfig) {
                item.height = item.width = ObjectConfig.getTestSizeForObject(object);
            } else if (item.hasOwnProperty("source")) {
                item.source = ObjectConfig.getSourceForObject(object);
            }

            if (item.hasOwnProperty("planet")) {
                item.planet = object;
            }
            if (item.hasOwnProperty("ship")) {
                item.ship = object;
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

    Rectangle {
        id: testBorder
        visible: testConfig && objectLoader.status === Loader.Ready
        anchors.fill: objectLoader
        color: "transparent"
        border.color: "green"
        border.width: 2
    }

    Component {
        id: defaultComponent
        AnimatedImage {
            cache: false
            MouseArea {
                id: item
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
                onDoubleClicked: mouse => {
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
            id: shipImage
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
                NumberAnimation {
                    duration: 2000
                }
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

        objectLoader.sourceComponent = getComponentForObject(object);
        positioning = ObjectConfig.getPositioningForObject(object);
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
