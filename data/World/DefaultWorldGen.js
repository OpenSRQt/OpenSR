var context = World.context;

function weapon(SoundPath, preview, weaponAnim, typeWeapon, radius, hitPoints) {
    var style = World.WeaponStyle();

    style.SoundPath = SoundPath;
    style.preview = preview;
    style.weaponAnim = weaponAnim;
    style.typeWeapon = typeWeapon;
    style.radius = radius;
    style.hitPoints = hitPoints;

    var weapon = World.Weapon(context);
    weapon.style = style;
    return weapon
}
function genRace(idName, name, icon, color, sound) {
    var style = World.RaceStyle();

    style.icon = icon;
    style.color = color;
    style.sound = sound;

    var race = World.Race(context);
    race.objectName = idName;
    race.name = name;
    race.style = style;

    return race
}

function genSector(idName, name, position) {
    var sector = World.Sector(context);
    sector.objectName = idName;
    sector.name = name;
    return sector;
}

function genSystem(idName, sector, name, size, star, starColor, bg) {
    var style = World.PlanetarySystemStyle();

    style.background = bg;
    style.star = star;
    style.starColor = starColor;

    var system = World.PlanetarySystem(sector);
    system.objectName = idName;
    system.name = name;
    system.size = size;
    system.style = style;
    return system;
}

var asteroidStyles = {};

function createAsteroidStyles() {
    var datStyles = Engine.datValue("Data.SE.Asteroid");
    for (var k in datStyles)
    {
        style = World.AsteroidStyle();
        style.texture = "dat:/" + datStyles[k]["Image"];
        style.color = "#FFFFFF";
        asteroidStyles[k] = style;
    }
}

function getPlanetStyleFromDat(id, isInhabitedPlanet) {
    var planet = Engine.datValue("Data.SE.Planet." + id)
    var color;
    if ("AtmColor" in planet) {
        var rgb = planet["AtmColor"].split(',')
        color = Qt.rgba(rgb[0] / 255.0, rgb[1] / 255.0, rgb[2] / 255.0, 1.0).toString()
    }
    else
        color = Qt.rgba(1.0, 1.0, 1.0, 1.0).toString()

    var splitted, cloud0 = "", cloud1 = "", cloud0Speed = 1.0, cloud1Speed = 1.0
    if ("Cloud0" in planet) {
        splitted = planet["Cloud0"].split(',')
        cloud0 = "dat:/" + splitted[1]
        cloud0Speed = parseFloat(splitted[0])
    }
    if ("Cloud1" in planet) {
        splitted = planet["Cloud1"].split(',')
        cloud1 = "dat:/" + splitted[1]
        cloud1Speed = parseFloat(splitted[0])
    }

    var style = isInhabitedPlanet
        ? World.InhabitedPlanetStyle()
        : World.DesertPlanetStyle();

    if(isInhabitedPlanet) {
        style.affiliation = "test affiliation";
    }

    style.background = "res:/DATA/PlanetBG/2/" + planet["BG"] + ".gi";
    style.surface  = "dat:/" + planet["Image"];
    style.cloud0 = cloud0;
    style.cloud1 = cloud1;
    style.atmosphere = color
    style.radius = planet["Radius"];
    return style
}

genRace("Race.People", "Race.Name.People", "dat:/Bm.Race.Emblem.2People", "#0030BD", "")
genRace("Race.Feyan", "Race.Name.Fei", "dat:/Bm.Race.Emblem.2Fei", "#E03BFF", "")
genRace("Race.Gaal", "Race.Name.Gaal", "dat:/Bm.Race.Emblem.2Gaal", "#FFDB0A", "")
genRace("Race.Peleng", "Race.Name.Peleng", "dat:/Bm.Race.Emblem.2Peleng", "#EF0000", "")
genRace("Race.Maloq", "Race.Name.Maloc", "dat:/Bm.Race.Emblem.2Maloc", "#607F16", "")
genRace("Race.Blazer", "Race.Name.Kling", "dat:/Bm.Race.Emblem.2Blazer", "#44BBA1", "")
genRace("Race.Keller", "Race.Name.Kling", "dat:/Bm.Race.Emblem.2Keller", "#44BBA1", "")
genRace("Race.Terron", "Race.Name.Kling", "dat:/Bm.Race.Emblem.2Terron", "#44BBA1", "")

createAsteroidStyles();

sector = genSector("Sector.Caragon", "Constellations.Name.3")
var system = genSystem("System.Solar", sector, "Star.02", 10000, "dat:/Bm.Star.00a", "#FFFFFF", "dat:/Bm.BGO.bg00")

var styleCount = Object.keys(asteroidStyles).length
var i = 0
for(var k in asteroidStyles)
{
    var asteroid = World.Asteroid(system);
    asteroid.objectName = "Asteroid." + k;
    asteroid.style = asteroidStyles[k];
    asteroid.period = 15;
    asteroid.semiAxis.x = 1500 + i * 10;
    asteroid.semiAxis.y = 1000;
    asteroid.angle = (i / styleCount) * 2 * 3.1415;
    i++
}

var planet = World.DesertPlanet(system);
planet.name = "PlanetName.Solar.2"
planet.style = getPlanetStyleFromDat("198", false)
planet.period = 15;
planet.angle = 3.1415 / 4;
planet.position = Qt.point(355, -222);

var planet = World.InhabitedPlanet(system);
planet.name = "PlanetName.Solar.1"
planet.style = getPlanetStyleFromDat("100", true)
planet.period = 15;
planet.angle = 3.1415 / 4;
planet.position = Qt.point(355, 222);

function initStation(obj, kind) {
    switch (kind) {
    case "ranger":
    case "rangerCenter":
    case "rc":
        obj.style = World.StationStyle();
        obj.style.texture = "res:/DATA/Ruins/RC.gai";
        obj.stationKind = World.StationKind.RangerCenter;
    }
}

var rangerCenter1 = World.SpaceStation(system);
initStation(rangerCenter1, "ranger")
rangerCenter1.position = Qt.point(-400, -100);

function shipStyleByAffiliation(ship) {
    var raceStr = "";
    switch (ship.affiliation) {
        case World.ShipAffiliation.Gaal: raceStr = "People"; break;
        case World.ShipAffiliation.Fei: raceStr = "Fei"; break;
        case World.ShipAffiliation.People: raceStr = "People"; break;
        case World.ShipAffiliation.Peleng: raceStr = "Peleng"; break;
        case World.ShipAffiliation.Maloc: raceStr = "Maloc"; break;
        default: return null;
    }
    var rankStr = "";
    switch (ship.rank) {
        case World.ShipRank.Diplomat: rankStr = "Diplomat"; break;
        case World.ShipRank.Liner: rankStr = "Liner"; break;
        case World.ShipRank.Ranger: rankStr = "Ranger"; break;
        case World.ShipRank.Pirate: rankStr = "Pirate"; break;
        case World.ShipRank.Warrior: rankStr = "Warrior"; break;
        case World.ShipRank.Transport: rankStr = "Transport"; break;
        default: return null;
    }

    var info = Engine.datValue("Data.SE.Ship." + raceStr + "." + rankStr);
    var style = World.ShipStyle();
    style.texture = "dat:/" + info.Image; // animated
    return style
}

var container = World.Container(context);
var weapon1 = weapon("DATA/PQI/Weapon/0/", "res:/DATA/Items/2W01", "res:/DATA/AB/2W01", "energy", 200, 12);
var weapon2 = weapon("Data/PQI/Weapon/0/", "res:/DATA/Items/2W01", "res:/DATA/AB/2W02", "fragment", 300, 20);
container.addWeapon(weapon1, 0);
container.addWeapon(weapon2, 1);

var ship1 = World.Ship(context);
ship1.position    = Qt.point(-300, -300);
ship1.affiliation = World.ShipAffiliation.People;
ship1.rank        = World.ShipRank.Diplomat;
ship1.style       = shipStyleByAffiliation(ship1);
ship1.angle       = 0;
ship1.structure   = 400;
ship1.activeWeapon = null;
ship1.style.width = 64;

var ship2 = World.Ship(system);
ship2.affiliation = World.ShipAffiliation.Fei;
ship2.rank        = World.ShipRank.Pirate;
ship2.style       = shipStyleByAffiliation(ship2);
ship2.angle       = 0;
ship2.structure   = 30;
ship2.activeWeapon = null;
ship2.style.width = 64;
ship2.position    = Qt.point(-300, 300);

context.container = container;
context.playerShip = ship1;
context.objectToShoot = null;
context.planetToEnter = null;
context.currentSystem = system;

World.saveWorld("/tmp/test.osr");