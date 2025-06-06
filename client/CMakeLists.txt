project(Lights_Client)

set(CMAKE_CXX_STANDARD 23)

set (SOURCES
        main.cpp
        network/client.cpp

        game/library/abilities/ability_system_component.cpp
        game/library/abilities/ability/jump_ability.cpp

        game/library/layers/ui_layer.cpp

        game/library/objects/ui/input/button.cpp
        game/library/objects/ui/input/text_input.cpp
        game/library/objects/player/player.cpp
        game/library/objects/ui/text/text_label.cpp
        game/library/objects/tilemap.cpp

        game/scenes/mmo/mmo_scene.cpp
        game/scenes/mmo/layers/mmo_character_select.cpp
        game/scenes/mmo/layers/mmo_game.cpp
        game/scenes/mmo/layers/mmo_title_screen.cpp

        game/scenes/physics_testing/physics_testing_scene.cpp
        game/scenes/physics_testing/layers/physics_testing_layer.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

# if set_base_dir(target) is defined, call it
if (COMMAND set_base_dir)
    set_base_dir(${PROJECT_NAME})
endif()

target_link_libraries(${PROJECT_NAME} Lights_Shared_Lib)
target_compile_definitions(Lights_Shared_Lib PRIVATE OZZ_RENDERER_OPENGL OZZ_WINDOW_GLFW)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_dependencies(${PROJECT_NAME} CopyAssets)
add_custom_target(CopyAssets ALL
        COMMAND ${CMAKE_COMMAND} -E echo "Copying assets..."
)

# Copy assets
set(ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)
add_custom_command(TARGET CopyAssets PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ASSETS_DIR}/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)