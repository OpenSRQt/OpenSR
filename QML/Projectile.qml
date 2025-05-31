import QtQuick 2.3
import OpenSR 1.0
import OpenSR.World 1.0

Item {
    id: projectileFactory
    width: 40
    height: 40
    visible: true

    Component {
        id: projectileComponent

        Item {
            id: projectile
            z: 2
            AnimatedImage {
                source: "res:/DATA/ABItem/W03_S.gai";
                width: 100
                height: 100
                cache: true
                anchors.centerIn: parent
            }
            property real directionAngle: 0
            rotation: directionAngle
            property real speed: 1
            property WorldObject target
            property bool active: true;

            Behavior on rotation {
                SmoothedAnimation {
                    duration: 100
                    velocity: 360
                }
            }
            PropertyAnimation on x {
                id: xAnim
                duration: 1000
                easing.type: Easing.Linear
                to: projectile.target.position.x
                running: projectile.active
            }
            PropertyAnimation on y {
                id: yAnim
                duration: 1000
                easing.type: Easing.Linear
                to: projectile.target.position.y
                running: projectile.active
            }

            Timer {
                id: moveTimer
                interval: 16
                running: true
                repeat: true

                onTriggered: {
                    if (!target || !target.position) {
                        destroyProjectile();
                        return;
                    }
                    var dx = target.position.x - (x + width/2);
                    var dy = target.position.y - (y + height/2);
                    var distance = Math.sqrt(dx*dx + dy*dy);
                    if (distance < 10) {
                        context.damageObject();
                        destroyProjectile();
                        return;
                    }
                    projectile.directionAngle = Math.atan2(dy, dx) * (180 / Math.PI);
                }
            }

            function destroyProjectile() {
                active = false;
                moveTimer.stop();
                destroy();
            }
        }
    }

    function createProjectile(startX, startY, target) {
        let projectile = projectileComponent.createObject(spaceNode, {
            x: startX,
            y: startY,
            target: target
        })
        return projectile
    }
}