import QtQuick
import OpenSR 1.0

Item {
    id: menu
    anchors.fill: parent
    property bool testConfig: isTestMode

    function updateBackgroundAnim() {
        bgToAnim.to = -background.width + menu.width;
        bgFromAnim.from = -background.width + menu.width;
        bgAnim.restart();
    }

    Connections {
        function onHeightChanged() {
            background.height = menu.height;
            updateBackgroundAnim();
        }
    }

    Image {
        id: background
        visible: !testConfig
        source: testConfig ? "" : "res:/DATA/FormMain3/2bg.gi"
        fillMode: Image.PreserveAspectCrop
        SequentialAnimation on x {
            id: bgAnim
            loops: Animation.Infinite
            PropertyAnimation {
                id: bgToAnim
                duration: 50000
                from: 0
            }
            PropertyAnimation {
                id: bgFromAnim
                duration: 50000
                to: 0
            }
        }
        onStatusChanged: {
            if (status == Image.Ready)
                updateBackgroundAnim();
        }
    }
    Image {
        visible: !testConfig
        source: testConfig ? "" : "res:/DATA/FormMain3/2Planet.gi"
        anchors.left: parent.left
        anchors.bottom: parent.bottom
    }
    GAIAnimatedImage {
        id: grid
        visible: !testConfig
        sources: testConfig ? null : ["res:/DATA/FormMain2/2AnimMain.gai"]
        speed: 0.75
        anchors.fill: parent

        onCurrentFrameChanged: {
            if (currentFrame == 0)
                gridPauseAnim.restart();
        }
        SequentialAnimation {
            id: gridPauseAnim

            PropertyAction {
                target: grid
                property: "playing"
                value: false
            }
            PauseAnimation {
                duration: 10000
            }
            PropertyAction {
                target: grid
                property: "playing"
                value: true
            }
        }
    }
    GAIAnimatedImage {
        id: animLine
        visible: !testConfig
        sources: testConfig ? null : ["res:/DATA/FormMain2/2AnimLine.gai"]
        speed: 0.5
        width: parent.width
        anchors.right: parent.right
        anchors.top: parent.top
    }
    Image {
        visible: !testConfig
        source: testConfig ? "" : "res:/DATA/FormMain3/2caption.gi"
        anchors.top: parent.top
        anchors.horizontalCenter: animLine.right
        anchors.topMargin: 20
        anchors.horizontalCenterOffset: -310 * animLine.width / animLine.implicitWidth
    }
    Item {
        width: childrenRect.width
        height: childrenRect.height

        Button {
            id: newButton
            text: testConfig ? "New Game" : ""
            normalImage: testConfig ? "" : "res:/DATA/FormMain2/2ButNewN.gi"
            hoveredImage: testConfig ? "" : "res:/DATA/FormMain2/2ButNewA.gi"
            downImage: testConfig ? "" : "res:/DATA/FormMain2/2ButNewD.gi"
            onClicked: newGame()
        }
        Button {
            id: loadButton
            text: testConfig ? "Load" : ""
            normalImage: testConfig ? "" : "res:/DATA/FormMain2/2ButLoadN.gi"
            hoveredImage: testConfig ? "" : "res:/DATA/FormMain2/2ButLoadA.gi"
            downImage: testConfig ? "" : "res:/DATA/FormMain2/2ButLoadD.gi"
            anchors.top: newButton.bottom
            anchors.topMargin: 10
            onClicked: loadGame()
        }
        Button {
            id: settingsButton
            text: testConfig ? "Settings" : ""
            normalImage: testConfig ? "" : "res:/DATA/FormMain2/2ButSettingsN.gi"
            hoveredImage: testConfig ? "" : "res:/DATA/FormMain2/2ButSettingsA.gi"
            downImage: testConfig ? "" : "res:/DATA/FormMain2/2ButSettingsD.gi"
            anchors.top: loadButton.bottom
            anchors.topMargin: 10
        }
        Button {
            id: recordsButton
            text: testConfig ? "Recoreds" : ""
            normalImage: testConfig ? "" : "res:/DATA/FormMain2/2ButRecordsN.gi"
            hoveredImage: testConfig ? "" : "res:/DATA/FormMain2/2ButRecordsA.gi"
            downImage: testConfig ? "" : "res:/DATA/FormMain2/2ButRecordsD.gi"
            anchors.top: settingsButton.bottom
            anchors.topMargin: 10
        }
        Button {
            id: aboutButton
            text: testConfig ? "About" : ""
            normalImage: testConfig ? "" : "res:/DATA/FormMain2/2ButAboutN.gi"
            hoveredImage: testConfig ? "" : "res:/DATA/FormMain2/2ButAboutA.gi"
            downImage: testConfig ? "" : "res:/DATA/FormMain2/2ButAboutD.gi"
            anchors.top: recordsButton.bottom
            anchors.topMargin: 10
        }
        Button {
            id: exitButton
            text: testConfig ? "Exit" : ""
            normalImage: testConfig ? "" : "res:/DATA/FormMain2/2ButExitN.gi"
            hoveredImage: testConfig ? "" : "res:/DATA/FormMain2/2ButExitA.gi"
            downImage: testConfig ? "" : "res:/DATA/FormMain2/2ButExitD.gi"
            anchors.top: aboutButton.bottom
            anchors.topMargin: 10
            onClicked: Qt.quit()
        }
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 100
        anchors.right: parent.right
        anchors.rightMargin: 100
    }

    GAIAnimatedImage {
        id: animation
        visible: !testConfig
        sources: testConfig ? null : ["res:/DATA/FormMain3/2Ship1.gai", "res:/DATA/FormMain3/2Ship2.gai", "res:/DATA/FormMain3/2Ship3.gai"]
        speed: 1.5
        anchors.left: parent.left
        anchors.bottom: parent.bottom
    }

    Button {
        id: questButton
        normalImage: testConfig ? "" : "res:/DATA/FormLoadRobot/2LoadQuestN.gi"
        hoveredImage: testConfig ? "" : "res:/DATA/FormLoadRobot/2LoadQuestA.gi"
        downImage: testConfig ? "" : "res:/DATA/FormLoadRobot/2LoadQuestD.gi"
        Text {
            id: questButtonText
            //% "Text quests"
            text: testConfig ? "" : qsTrId("FormMain.LQuest")
            color: "#FFC710"
            anchors.top: questButton.bottom
            anchors.horizontalCenter: questButton.horizontalCenter
        }
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.bottomMargin: questButtonText.height + 10
        anchors.leftMargin: 200
        onClicked: {
            gameScreen.createObjectFromURL("qrc:/OpenSR/QuestSelectionMenu.qml", menu, "questSelectionRequest");
        }
    }

    Music {
        id: music
        source: testConfig ? "" : "res:/Music/SPECIAL/SpaceIsCalling.dat"
        Component.onCompleted: {
            if (!testConfig) {
                music.play();
            }
        }
    }

    function startQuest(id) {
        destroyAndChangeScreen("qrc:/OpenSR/QuestPlayer.qml", {
            "questID": id
        });
    }

    function newGame() {
        if (testConfig) {
            World.generateWorld("res:/World/TestWorldGen.js");
        } else {
            World.generateWorld("res:/World/DefaultWorldGen.js");
        }

        menu.destroy();
        changeScreen("qrc:/OpenSR/SpaceView.qml", {
            "system": World.context.currentSystem
        });
    }

    function loadGame() {
        World.loadWorld("/tmp/test.osr");
        menu.destroy();
        changeScreen("qrc:/OpenSR/SpaceView.qml", {
            "system": World.context.currentSystem
        });
    }

    function componentObjectCreated(object, id) {
        if (id === "questSelectionRequest") {
            object.anchors.horizontalCenter = menu.horizontalCenter;
            object.anchors.verticalCenter = menu.verticalCenter;
            object.questSelected.connect(startQuest);
        }
    }
}
