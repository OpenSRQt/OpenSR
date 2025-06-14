var pendingComponents = []
var components = {}
var failedComponents = {}
var objectRequests = []

function loadComponent(url) {
    if((url in components) || (url in failedComponents))
        return;
    var comp = Qt.createComponent(url);
    //components[url] = comp;
    pendingComponents.push(comp);
    comp.statusChanged.connect(finishComponentLoad);
    finishComponentLoad();
}

function processObjectRequests() {
    var newObjectRequests = []
    var obj = null;
    for (var i = 0; i < objectRequests.length; i++) {
        var req = objectRequests[i];
        // console.log("object request " + i + ": " + req);
        if (req[0] in components) {
            obj = components[req[0]].createObject(req[1], req[3]);
            req[1].componentObjectCreated(obj, req[2]);
        }
        else if (req[0] in failedComponents) {
        }
        else {
            newObjectRequests.push(objectRequests[i]);
        }
    }
    objectRequests = newObjectRequests
    return obj
}

function finishComponentLoad() {
    var newPending = []
    for (var i = 0; i < pendingComponents.length; i++) {
        var comp = pendingComponents[i];
        if (comp.status == Component.Ready) {
            components[comp.url] = comp;
        } else if (comp.status == Component.Error) {
            failedComponents[comp.url] = comp
        }
        else {
            newPending.push(comp);
        }
    }
    pendingComponents = newPending;
    return processObjectRequests();
}

function createObjectFromURL(url, parent, id, properties) {
    loadComponent(url)
    objectRequests.push([url, parent, id, properties]);
    return processObjectRequests();
}
