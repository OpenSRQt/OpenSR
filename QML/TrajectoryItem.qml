import QtQuick 2.3
import OpenSR 1.0
import OpenSR.World 1.0

Item {
    property SpaceObject object
    property SpaceObject oldObject
    property rect visibleRect
    property var curves
    property bool testConfig: isTestMode

    Item {
        id: trajectory
    }

    Component {
        id: testCurveComponent
        TexturedBezierCurve {
            source: ""
            minStep: 20
        }
    }

    Component {
        id: testStepComponent
        Rectangle {
            color: "transparent"
            border.color: "green"
            width: 12
            height: 12
            radius: 6
            x: -6
            y: -6
        }
    }

    Component {
        id: testFirstStepComponent
        Rectangle {
            color: "transparent"
            border.color: "red"
            width: 16
            height: 16
            radius: 8
            x: -8
            y: -8
        }
    }

    onObjectChanged: {
        if (oldObject)
            oldObject.trajectoryChanged.disconnect(updateTrajectory);

        if (object)
            object.trajectoryChanged.connect(updateTrajectory);

        oldObject = object;

        if (object)
            updateTrajectory();
    }

    function pointVisible(p) {
        return !((p.x < visibleRect.x) || (p.y < visibleRect.y) || (p.x > (visibleRect.x + visibleRect.width)) || (p.y > (visibleRect.y + visibleRect.height)));
    }

    function updateTrajectory() {
        for (var i in trajectory.children) {
            trajectory.children[i].destroy();
        }

        if (!object || !object.trajectory) {
            return;
        }

        curves = object.trajectory;

        var visibleCurves = [];
        var firstIncluded = false;
        for (i in curves) {
            if (pointVisible(curves[i].p0) || pointVisible(curves[i].p3)) {
                if (i == 0)
                    firstIncluded = true;
                visibleCurves.push(curves[i]);
            }
        }

        if (testConfig) {
            createTestTrajectory(visibleCurves, firstIncluded);
        } else {
            createRealTrajectory(visibleCurves, firstIncluded);
        }
    }

    function createTestTrajectory(visibleCurves, firstIncluded) {
        for (var i in visibleCurves) {
            testCurveComponent.createObject(trajectory, {
                curve: visibleCurves[i]
            });

            var stepComponent = ((i == 0) && firstIncluded) ? testFirstStepComponent : testStepComponent;

            var stepObj = stepComponent.createObject(trajectory);
            stepObj.x = visibleCurves[i].p3.x;
            stepObj.y = visibleCurves[i].p3.y;
        }
    }

    function createRealTrajectory(visibleCurves, firstIncluded) {
        var beizerQML = "import OpenSR 1.0; TexturedBezierCurve { source: \"dat:/Bm.PI.Path2\"; minStep: 20; }";
        var stepQML = "import QtQuick 2.3; Item { property alias source: image.source; Image { id: image; source: \"dat:/Bm.PI.Path3\"; x: -width/2; y: -height/2; } }";

        for (var i in visibleCurves) {
            var o = Qt.createQmlObject(beizerQML, trajectory);
            if ((i == 0) && firstIncluded)
                o.source = "dat:/Bm.PI.Path1";
            o.curve = visibleCurves[i];
        }

        for (i in visibleCurves) {
            var o = Qt.createQmlObject(stepQML, trajectory);
            if ((i == 0) && firstIncluded)
                o.source = "dat:/Bm.PI.Path4";
            o.x = visibleCurves[i].p3.x;
            o.y = visibleCurves[i].p3.y;
        }
    }
}
