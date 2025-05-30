if(BUILD_RESOURCE_VIEWER)
    add_subdirectory(ResourceViewer)
endif()

if(BUILD_PLANET_VIEWER)
    add_subdirectory(PlanetViewer)
endif()

if(BUILD_QUEST_PLAYER)
    add_subdirectory(QuestPlayer)
endif()

if(BUILD_DAT_TOOLS)
    add_subdirectory(DATTools)
endif()