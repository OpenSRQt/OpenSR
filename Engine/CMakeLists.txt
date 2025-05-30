find_package(Qt6 REQUIRED COMPONENTS Widgets Quick ShaderTools)
find_package(OpenAL REQUIRED)

add_library(engine SHARED
    Engine.cpp
    SoundManager.cpp
    Sound.cpp
    GAIAnimatedImage.cpp
    GAITexture.cpp
    ResourceManager.cpp
    QMLQuestPlayer.cpp
    Music.cpp
    MPG123MusicDecoder.cpp
    VorbisMusicDecoder.cpp
    TexturedPolyline.cpp
    TexturedBezierCurve.cpp
    Types.cpp
    QMLHelper.cpp
    DATTranslator.cpp
    SpaceMouseArea.cpp
    PlanetDrawer.cpp
    GAIMaterial.cpp
    GAIShader.cpp

    ${CMAKE_SOURCE_DIR}/include/OpenSR/Engine.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/SoundManager.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/Sound.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/GAIAnimatedImage.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/GAITexture.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/ResourceManager.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/QMLQuestPlayer.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/Music.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/MusicDecoder.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/TexturedPolyline.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/TexturedBezierCurve.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/Types.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/QMLHelper.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/PlanetDrawer.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/DATTranslator.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/SpaceMouseArea.h
    ${CMAKE_SOURCE_DIR}/include/OpenSR/PlanetDrawer.h

    GAIMaterial.h
    GAIShader.h
    ResourceManager_p.h
    SoundManager_p.h
    MPG123MusicDecoder.h
    VorbisMusicDecoder.h
)

qt6_add_shaders(engine "engine-shaders"
    BATCHABLE
    PREFIX
        "/"
    FILES
        "shaders/GAIShader.frag"
        "shaders/GAIShader.vert"
)

target_include_directories(engine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_include_directories(engine PRIVATE
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
    ${OPENGL_INCLUDE_DIR}
    ${OPENAL_INCLUDE_DIR}
)

target_link_libraries(engine PRIVATE
    RangerQt
    QM
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Quick
    Qt6::GuiPrivate
    ${OPENAL_LIBRARY}
)

target_compile_definitions(engine PRIVATE OPENSR_ENGINE_BUILD)

if(WIN32)
    set_target_properties(engine PROPERTIES PREFIX "")
endif()

add_executable(opensr main.cpp ${CMAKE_SOURCE_DIR}/QML/qml.qrc)

target_link_libraries(opensr PRIVATE
    Qt6::Widgets
    engine
)

install(TARGETS opensr RUNTIME DESTINATION bin)
