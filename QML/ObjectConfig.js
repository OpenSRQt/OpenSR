const ObjectTypes = {
    PlanetarySystem: "OpenSR::World::PlanetarySystem",
    Asteroid: "OpenSR::World::Asteroid", 
    DesertPlanet: "OpenSR::World::DesertPlanet",
    InhabitedPlanet: "OpenSR::World::InhabitedPlanet",
    SpaceStation: "OpenSR::World::SpaceStation",
    Ship: "OpenSR::World::Ship"
};

const ObjectSettings = {
    [ObjectTypes.PlanetarySystem]: {
        positioning: false,
        testSize: 100,
        borderColor: "yellow",
        getSource: function(obj) { return obj ? obj.style.star || "" : ""; }
    },
    [ObjectTypes.Asteroid]: {
        positioning: true,
        testSize: 16,
        borderColor: "green",
        getSource: function(obj) { return obj ? obj.style.texture || "" : ""; }
    },
    [ObjectTypes.DesertPlanet]: {
        positioning: true,
        testSize: 64, 
        borderColor: "brown",
        getSource: function(obj) { return obj ? obj.style.texture || "" : ""; }
    },
    [ObjectTypes.InhabitedPlanet]: {
        positioning: true,
        testSize: 64,
        borderColor: "blue",
        getSource: function(obj) { return obj ? obj.style.texture || "" : ""; }
    },
    [ObjectTypes.SpaceStation]: {
        positioning: true,
        testSize: 64,
        borderColor: "gray",
        getSource: function(obj) { return obj ? obj.style.texture || "" : ""; }
    },
    [ObjectTypes.Ship]: {
        positioning: true,
        testSize: 64,
        borderColor: "orange",
        getSource: function(obj) { return obj ? obj.style.texture || "" : ""; }
    }
};

function getObjectSettings(object) {
    if (!object) return null;
    
    const typeName = WorldManager.typeName(object.typeId);
    return ObjectSettings[typeName] || null;
}

function getPositioningForObject(obj) {
    const settings = getObjectSettings(obj);
    return settings ? settings.positioning : true;
}

function getTestSizeForObject(obj) {
    const settings = getObjectSettings(obj);
    return settings ? settings.testSize : 64;
}

function getBorderColorForObject(obj) {
    const settings = getObjectSettings(obj);
    return settings ? settings.borderColor : "green";
}

function getSourceForObject(obj) {
    const settings = getObjectSettings(obj);
    return settings && settings.getSource ? settings.getSource(obj) : "";
}