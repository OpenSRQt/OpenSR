import QtQuick 2.3
import OpenSR 1.0

Item {
    id: button

    property string text
    property bool testConfig: isTestMode

    property color textColor: "black"
    property int textStyle: Text.Normal
    property color textStyleColor: "black"
    property font textFont

    property string normalImage: "res:/ORC/ButtonN.sci"
    property string hoveredImage: "res:/ORC/ButtonA.sci"
    property string downImage: "res:/ORC/ButtonD.sci"
    property string enterSound: "res:/Sound/ButtonEnter.wav"
    property string leaveSound: "res:/Sound/ButtonLeave.WAV"
    property string clickSound: "res:/Sound/ButtonClick.wav"
    property bool sounded: true
    signal clicked

    property color testNormalColor: "lightgray"
    property color testHoveredColor: "gray"
    property color testDownColor: "darkgray"

    implicitWidth: testConfig ? 100 : (bgLoader.item ? bgLoader.item.implicitWidth : label.implicitWidth + 10)
    implicitHeight: testConfig ? 30 : (bgLoader.item ? bgLoader.item.implicitHeight : label.implicitHeight + 10)

    Loader {
        id: bgLoader
        anchors.fill: parent
        sourceComponent: testConfig ? testBackground : realBackground
    }

    Component {
        id: realBackground
        BorderImage {
            anchors.fill: parent
            source: (hoverArea.pressed && button.downImage != "") ? button.downImage : ((hoverArea.containsMouse && button.hoveredImage != "") ? button.hoveredImage : button.normalImage)
        }
    }

    Component {
        id: testBackground
        Rectangle {
            color: hoverArea.pressed ? button.testDownColor : hoverArea.containsMouse ? button.testHoveredColor : button.testNormalColor
            border.color: "darkgray"
            border.width: 1
            radius: 4
        }
    }

    Text {
        id: label
        anchors.centerIn: parent
        text: button.text
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        renderType: Text.NativeRendering

        color: button.textColor
        style: button.textStyle
        styleColor: button.textStyleColor
        font: button.textFont
    }

    Sound {
        id: cSnd
        source: !button.testConfig && button.sounded ? button.clickSound : "res:/"
    }
    Sound {
        id: eSnd
        source: !button.testConfig && button.sounded ? button.enterSound : "res:/"
    }
    Sound {
        id: lSnd
        source: !button.testConfig && button.sounded ? button.leaveSound : "res:/"
    }

    MouseArea {
        id: hoverArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            button.clicked();
        }
        onPressed: {
            if (!button.testConfig && button.sounded) {
                cSnd.play();
            }
        }
        onEntered: {
            if (!button.testConfig && button.sounded) {
                eSnd.play();
            }
        }
        onExited: {
            if (!button.testConfig && button.sounded) {
                lSnd.play();
            }
        }
    }
}
