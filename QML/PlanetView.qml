import QtQuick 2.3
import OpenSR 1.0
import OpenSR.World 1.0

Item {
    property InhabitedPlanet planet
    id: planetView
    anchors.fill: parent

    Rectangle {
        anchors.fill: parent
        visible: !planet
        color: "gray"
        Text {
            text: "placeholder"
            anchors.centerIn: parent
        }
    }

    Item {
        x: parent.width / 2
        y: parent.height / 2

        Image {
            id: bg
            source: planet ? planet.style.background : ""
            x: -width / 2
            y: -height / 2
            cache: false
        }
    }

    MouseArea {
        id: planetMouseArea
        anchors.fill: parent

        onClicked: {
        }
    }

    Button {
        id: exitPlanetButton
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        text: "Exit"
        onClicked: {
            planetView.destroy()
            context.playerShip.exitThePlace();
        }
    }
}