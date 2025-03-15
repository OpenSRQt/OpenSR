import QtQuick 2.0

Item {

    property string backLeftImage: {
        Engine.datValue("ML.R2.Ship." + raceStr + "." + rankStr);
        "res:/ORC/ButtonN.sci"
    }
    BorderImage {
        id: bg
        width: 410
        height: 617
        anchors.fill: parent
        source: backLeftImage
    }
}
