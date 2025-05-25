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
            Rectangle {
                width: 5
                height: 20
                color: "yellow"
                radius: 3
                anchors.centerIn: parent
                rotation: 90
            }
            property real directionAngle: 0
            rotation: directionAngle

            property real speed: 5
            property WorldObject target
            property bool active: true

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
                    var targetPos = target.position;
                    var projectileCenterX = x + width/2;
                    var projectileCenterY = y + height/2;

                    var dx = targetPos.x - projectileCenterX;
                    var dy = targetPos.y - projectileCenterY;
                    var distance = Math.sqrt(dx*dx + dy*dy);

                    if (distance < 10) {
                        console.log("Collision!");
                        context.damageObject();
                        destroyProjectile();
                        return;
                    }
                    projectile.directionAngle = Math.atan2(dy, dx) * (180 / Math.PI);
                    let directionX = dx / distance;
                    let directionY = dy / distance;
                    x += directionX * speed;
                    y += directionY * speed;
                }
            }

            function destroyProjectile() {
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