import QtQuick 2.3
import OpenSR 1.0
import OpenSR.World 1.0

Item {
    id: view
    property PlanetarySystem system
    property int speed: 500
    property int bgSpeed: 10
    property int scrollSize: 10
    property int radarViewWidth: 150
    property int maxScrollTime: 600

    property var trajectoryView
    property var object

    anchors.fill: parent

    MouseArea {
        id: spaceMouseOverlay
        anchors.fill: parent

        propagateComposedEvents: true

        onDoubleClicked: mouse => {
            if (!WorldManager.turnFinished) {
                return;
            }

            var positionInSpaceNode = mapToItem(spaceNode, mouse.x, mouse.y);
            context.playerShip.prepareToMove(positionInSpaceNode);
            showTrajectory(context.playerShip);
            WorldManager.startTurn();
            if(context.objectToShoot) fireProjectile(context.objectToShoot);
            hideTrajectory(context.playerShip);
        }

        onClicked: mouse => {
            if (!WorldManager.turnFinished) {
                return;
            }
            mouse.accepted = true;

            var positionInSpaceNode = mapToItem(spaceNode, mouse.x, mouse.y);
            context.playerShip.prepareToMove(positionInSpaceNode);
            showTrajectory(context.playerShip);
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "black"
    }

    Item {
        x: parent.width / 2
        y: parent.height / 2

        Image {
            id: bg
            source: system ? system.style.background : ""
            x: -width / 2
            y: -height / 2
            cache: false
        }

        Item {
            id: spaceNode
            //transform: Scale {xScale: 0.25; yScale: 0.25}
        }
    }

    Item {
        id: radarView
        anchors.right: parent.right
        anchors.top: parent.top

        width: radarViewWidth
        height: width

        Image {
            source: "res:/DATA/PanelSpace2/1RadarA.gi"
            anchors.fill: parent
            cache: true
        }
        Button {
            id: radarCenterButton
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            normalImage: "res:/DATA/PanelSpace2/1CenterN.gi"
            hoveredImage: "res:/DATA/PanelSpace2/1CenterA.gi"
            downImage: "res:/DATA/PanelSpace2/1CenterD.gi"
            onClicked: console.log("Centering not implemented")
        }
    }

    Item {
        id: bottomPanel
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: 100
        Image {
            source: "res:/DATA/PanelMain2/2BG.gi"
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.fill: parent

            cache: true
        }
    }

    onSystemChanged: {
        for (var i in spaceNode.children) {
            spaceNode.children[i].destroy();
        }
        CursorManager.setWindow(view);
        CursorManager.changeCursor(CursorType.Main);
        if (!system)
            return;

        var component = Qt.createComponent("SpaceObjectItem.qml");

        var o = component.createObject(spaceNode, {
            object: system,
            mouseDelta: 50
        });
        o.entered.connect(showDebugTooltip);
        o.exited.connect(hideDebugTooltip);
        for (var c in system.children) {
            o = component.createObject(spaceNode, {
                object: system.children[c]
            });

            o.entered.connect(showDebugTooltip);
            o.exited.connect(hideDebugTooltip);
            o.entered.connect(showTrajectory);
            o.exited.connect(hideTrajectory);
        }

        var shipObject = component.createObject(spaceNode, {
            object: World.context.playerShip
        });

        shipObject.entered.connect(showDebugTooltip);
        shipObject.exited.connect(hideDebugTooltip);

        var trajComponent = Qt.createComponent("TrajectoryItem.qml");
        trajectoryView = trajComponent.createObject(spaceNode, {
            object: null,
            visible: false
        });
    }

    Projectile {
        id: factory
        anchors.fill: parent
    }

    function fireProjectile(object) {
        if(object) {
            let startX = context.playerShip.position.x;
            let startY = context.playerShip.position.y;

            var projectile = factory.createProjectile(
                startX,
                startY,
                object
            )
        }
    }

    DebugTooltip {
        id: debug
        visible: false
    }

    function showTrajectory(object) {
        trajectoryView.visibleRect = spaceNode.mapFromItem(view, 0, 0, view.width, view.height);
        if (trajectoryView.object !== object) {
            trajectoryView.object = object;
        }
        trajectoryView.visible = true;
    }
    function hideTrajectory(object) {
        if (trajectoryView.object === object) {
            trajectoryView.visible = false;
        }
    }

    function showDebugTooltip(object) {
        debug.object = object;
        debug.visible = true;
    }
    function hideDebugTooltip(object) {
        if (object === debug.object) {
            debug.object = null;
            debug.visible = false;
        }
    }

    ParallelAnimation {
        id: hAnim
        PropertyAnimation {
            id: hBgAnim
            target: bg
            property: "x"
            duration: maxScrollTime * 1000
            alwaysRunToEnd: false
        }
        PropertyAnimation {
            id: hFgAnim
            target: spaceNode
            property: "x"
            duration: maxScrollTime * 1000
            alwaysRunToEnd: false
        }
    }
    ParallelAnimation {
        id: vAnim
        PropertyAnimation {
            id: vBgAnim
            target: bg
            property: "y"
            duration: maxScrollTime * 1000
            alwaysRunToEnd: false
        }
        PropertyAnimation {
            id: vFgAnim
            target: spaceNode
            property: "y"
            duration: maxScrollTime * 1000
            alwaysRunToEnd: false
        }
    }

    MouseArea {
        id: leftHoverArea

        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        anchors.topMargin: scrollSize
        anchors.bottomMargin: scrollSize

        width: scrollSize

        hoverEnabled: true
        onEntered: {
            hBgAnim.to = bg.x + maxScrollTime * bgSpeed;
            hFgAnim.to = spaceNode.x + maxScrollTime * speed;
            hAnim.start();
        }
        onExited: {
            hAnim.stop();
        }
    }
    MouseArea {
        id: rightHoverArea

        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        anchors.topMargin: scrollSize
        anchors.bottomMargin: scrollSize

        width: scrollSize

        hoverEnabled: true
        onEntered: {
            hBgAnim.to = bg.x - maxScrollTime * bgSpeed;
            hFgAnim.to = spaceNode.x - maxScrollTime * speed;
            hAnim.start();
        }
        onExited: {
            hAnim.stop();
        }
    }
    MouseArea {
        id: topHoverArea

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        anchors.leftMargin: scrollSize
        anchors.rightMargin: scrollSize

        height: scrollSize

        hoverEnabled: true
        onEntered: {
            vBgAnim.to = bg.y + maxScrollTime * bgSpeed;
            vFgAnim.to = spaceNode.y + maxScrollTime * speed;
            vAnim.start();
        }
        onExited: {
            vAnim.stop();
        }
    }
    MouseArea {
        id: bottomHoverArea

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        anchors.leftMargin: scrollSize
        anchors.rightMargin: scrollSize

        height: scrollSize

        hoverEnabled: true
        onEntered: {
            vBgAnim.to = bg.y - maxScrollTime * bgSpeed;
            vFgAnim.to = spaceNode.y - maxScrollTime * speed;
            vAnim.start();
        }
        onExited: {
            vAnim.stop();
        }
    }
    MouseArea {
        id: topleftHoverArea

        anchors.left: parent.left
        anchors.top: parent.top

        width: scrollSize
        height: scrollSize

        hoverEnabled: true
        onEntered: {
            hBgAnim.to = bg.x + maxScrollTime * bgSpeed / Math.sqrt(2);
            hFgAnim.to = spaceNode.x + maxScrollTime * speed / Math.sqrt(2);
            vBgAnim.to = bg.y + maxScrollTime * bgSpeed / Math.sqrt(2);
            vFgAnim.to = spaceNode.y + maxScrollTime * speed / Math.sqrt(2);
            hAnim.start();
            vAnim.start();
        }
        onExited: {
            hAnim.stop();
            vAnim.stop();
        }
    }
    MouseArea {
        id: toprightHoverArea

        anchors.right: parent.right
        anchors.top: parent.top

        width: scrollSize
        height: scrollSize

        hoverEnabled: true
        onEntered: {
            hBgAnim.to = bg.x - maxScrollTime * bgSpeed / Math.sqrt(2);
            hFgAnim.to = spaceNode.x - maxScrollTime * speed / Math.sqrt(2);
            vBgAnim.to = bg.y + maxScrollTime * bgSpeed / Math.sqrt(2);
            vFgAnim.to = spaceNode.y + maxScrollTime * speed / Math.sqrt(2);
            hAnim.start();
            vAnim.start();
        }
        onExited: {
            hAnim.stop();
            vAnim.stop();
        }
    }
    MouseArea {
        id: bottomleftHoverArea

        anchors.left: parent.left
        anchors.bottom: parent.bottom

        width: scrollSize
        height: scrollSize

        hoverEnabled: true
        onEntered: {
            hBgAnim.to = bg.x + maxScrollTime * bgSpeed / Math.sqrt(2);
            hFgAnim.to = spaceNode.x + maxScrollTime * speed / Math.sqrt(2);
            vBgAnim.to = bg.y - maxScrollTime * bgSpeed / Math.sqrt(2);
            vFgAnim.to = spaceNode.y - maxScrollTime * speed / Math.sqrt(2);
            hAnim.start();
            vAnim.start();
        }
        onExited: {
            hAnim.stop();
            vAnim.stop();
        }
    }
    MouseArea {
        id: bottomrightHoverArea

        anchors.right: parent.right
        anchors.bottom: parent.bottom

        width: scrollSize
        height: scrollSize

        hoverEnabled: true
        onEntered: {
            hBgAnim.to = bg.x - maxScrollTime * bgSpeed / Math.sqrt(2);
            hFgAnim.to = spaceNode.x - maxScrollTime * speed / Math.sqrt(2);
            vBgAnim.to = bg.y - maxScrollTime * bgSpeed / Math.sqrt(2);
            vFgAnim.to = spaceNode.y - maxScrollTime * speed / Math.sqrt(2);
            hAnim.start();
            vAnim.start();
        }
        onExited: {
            hAnim.stop();
            vAnim.stop();
        }
    }

    // Get Gun, Get Gun 0 and Get Gun 1?
    // Ideally, this buttons should be on the bottom panel
    Column {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        spacing: 5

        Button {
            id: getGun0
            text: "Get Gun 0"
            onClicked: {
                if (!WorldManager.turnFinished) {
                    return;
                }
                context.isChoosingToShoot = context.setActiveWeapon(0);
                if(context.isChoosingToShoot) {
                    CursorManager.changeCursor(CursorType.FireSmall)
                } else {
                    CursorManager.changeCursor(CursorType.Main)
                }
            }
        }

        Button {
            id: getGun1
            text: "Get Gun 1"
            onClicked: {
                if (!WorldManager.turnFinished) {
                    return;
                }
                context.isChoosingToShoot = context.setActiveWeapon(1);
                if(context.isChoosingToShoot) {
                    CursorManager.changeCursor(CursorType.FireSmall)
                } else {
                    CursorManager.changeCursor(CursorType.Main)
                }
            }
        }
    }

    // TODO: Put it into right place of bottom panel
    Button {
        id: showShipButton
        anchors.bottom: parent.bottom
        anchors.right: turnButton.left
        anchors.bottomMargin: 13
        anchors.rightMargin: 64
        sounded: false
        normalImage: "res:/DATA/FormAB2/2ShipN.gi"
        hoveredImage: "res:/DATA/FormAB2/2ShipA.gi"
        downImage: "res:/DATA/FormAB2/2ShipD.gi"
    }

    Button {
        id: turnButton
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        sounded: false
        normalImage: "res:/DATA/PanelMain2/2TurnN.gi"
        hoveredImage: "res:/DATA/PanelMain2/2TurnA.gi"
        downImage: "res:/DATA/PanelMain2/2TurnD.gi"
        onClicked: {
            if (!WorldManager.turnFinished) {
                return;
            }
            WorldManager.startTurn();
            if(context.objectToShoot) fireProjectile(context.objectToShoot);
            context.isChoosingToShoot = false;
            hideTrajectory(context.playerShip);
        }
    }
}
