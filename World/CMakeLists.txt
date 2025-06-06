find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Quick Qml)

add_library(world SHARED
    Artefact.cpp
    Asteroid.cpp
    CargoHook.cpp
    Container.cpp
    DefenceGenerator.cpp
    DesertPlanet.cpp
    Droid.cpp
    Engine.cpp
    Equipment.cpp
    Goods.cpp
    Hull.cpp
    InhabitedPlanet.cpp
    Item.cpp
    MannedObject.cpp
    Micromodulus.cpp
    Planet.cpp
    PlanetarySystem.cpp
    Race.cpp
    Radar.cpp
    Scanner.cpp
    Sector.cpp
    Ship.cpp
    SpaceObject.cpp
    SpaceStation.cpp
    Tank.cpp
    TurnAnimation.cpp
    Weapon.cpp
    WorldContext.cpp
    WorldObject.cpp
    WorldPlugin.cpp
    WorldManager.cpp
    WorldBindings.cpp
    ResourceManager.cpp
    Resource.cpp

    Artefact.h
    Asteroid.h
    CargoHook.h
    Container.h
    DefenceGenerator.h
    DesertPlanet.h
    Droid.h
    Engine.h
    Equipment.h
    Goods.h
    Hull.h
    InhabitedPlanet.h
    Item.h
    MannedObject.h
    Micromodulus.h
    Planet.h
    PlanetarySystem.h
    Race.h
    Radar.h
    Scanner.h
    Sector.h
    Ship.h
    SpaceObject.h
    SpaceStation.h
    Tank.h
    TurnAnimation.h
    Weapon.h
    WorldContext.h
    WorldObject.h
    WorldPlugin.h
    WorldManager.h
    WorldBindings.h
    ResourceManager.h
    Resource.h
)

target_include_directories(world PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_definitions(world PRIVATE OPENSR_WORLD_BUILD)

target_link_libraries(world PRIVATE 
    engine 
    Qt6::Core 
    Qt6::Gui 
    Qt6::Widgets 
    Qt6::Quick 
    Qt6::Qml
)

IF(WIN32)
    SET_TARGET_PROPERTIES(world PROPERTIES PREFIX "")
ENDIF()
