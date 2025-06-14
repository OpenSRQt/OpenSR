import QtQuick 2.9
import QtQuick.Window 2.2
import OpenSR 1.0

Window {
    id: gameScreen
    width: 1024
    height: 768
    visible: true

    function createObjectFromURL(url, parent, id, properties) {
        properties = (typeof properties === 'undefined') ? {} : properties;
        return ScreenLoader.createObjectFromURL(url, parent, id, properties);
    }

    function changeScreen(url, properties) {
        properties = (typeof properties === 'undefined') ? {} : properties;
        var obj = createObjectFromURL(url, gameScreen, "screenRequest", properties);
        if(obj) {
            CursorManager.setWindow(obj);
            CursorManager.changeCursor(CursorType.Main);
        }
    }

    function destroyAndChangeScreen(url, properties) {
        var childItems = gameScreen.data || [];
        for (var i = childItems.length - 1; i >= 0; i--) {
            var child = childItems[i];
            if (child && child.hasOwnProperty("destroy")) {
                child.destroy();
            }
        }

        changeScreen(url, properties);
    }

    function componentObjectCreated() {
    }

    function quitComponent(obj) {
        if(obj) {
            CursorManager.setWindow(obj.parent);
            CursorManager.changeCursor(CursorType.Main);
            obj.destroy()
        }
    }
}
