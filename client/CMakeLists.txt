project(Lights_Client)

set(CMAKE_CXX_STANDARD 20)

set (SOURCES
        main.cpp
        network/client.cpp

        game/client_game.cpp

        game/rendering/buffer.cpp
        game/rendering/material.cpp
        game/rendering/render_pass.cpp
        game/rendering/renderer.cpp
        game/rendering/shader.cpp
        game/rendering/texture.cpp

        game/scene/main_menu/main_menu_scene.cpp
        game/scene/main_menu/objects/pepe.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

# if set_base_dir(target) is defined, call it
if (COMMAND set_base_dir)
    set_base_dir(${PROJECT_NAME})
endif()

target_link_libraries(${PROJECT_NAME} Lights_Shared_Lib)

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