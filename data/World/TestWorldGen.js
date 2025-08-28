var context = World.context;

var sector = World.Sector(context);

var system = World.PlanetarySystem(sector);

for(let i = 0; i < 20; i++)
{
    var asteroid = World.Asteroid(system);
    asteroid.objectName = "Asteroid." + i;
    asteroid.period = 15;
    asteroid.semiAxis.x = 1500 + i * 10;
    asteroid.semiAxis.y = 1000;
    asteroid.angle = (i / 20) * 2 * 3.1415;
}

var planet1 = World.DesertPlanet(system);
planet1.name = "PlanetName.Solar.2"
planet1.period = 15;
planet1.angle = 3.1415 / 4;
planet1.position = Qt.point(355, -222);

var planet2 = World.InhabitedPlanet(system);
planet2.name = "PlanetName.Solar.1"
planet2.period = 15;
planet2.angle = 3.1415 / 4;
planet2.position = Qt.point(355, 222);

var rangerCenter = World.SpaceStation(system);
rangerCenter.position = Qt.point(-400, -100);
rangerCenter.stationKind = World.StationKind.RangerCenter;

var ship1 = World.Ship(context);

ship1.position    = Qt.point(-300, -300);
ship1.affiliation = World.ShipAffiliation.People;
ship1.rank        = World.ShipRank.Diplomat;
ship1.angle       = 0;

context.playerShip = ship1;
context.planetToEnter = null;
context.currentSystem = system;

World.saveWorld("/tmp/test.osr");