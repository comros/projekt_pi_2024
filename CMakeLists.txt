include(FetchContent)

# CMAKE SETUP
cmake_minimum_required(VERSION 3.28)
project(projekt_pi_2024 LANGUAGES CXX)

set(CMAKE_EXE_LINKER_FLAGS "-static")
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# SFML import
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
)
FetchContent_MakeAvailable(SFML)

# ImGui import
FetchContent_Declare(imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG v1.89.9
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(imgui)
FetchContent_GetProperties(imgui SOURCE_DIR IMGUI_DIR)

# ImGui-SFML import
FetchContent_Declare(imgui-sfml
        GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
        GIT_TAG        2.6.x
)

# ImGui-SFML setup
set(IMGUI_DIR ${imgui_SOURCE_DIR})
option(IMGUI_SFML_FIND_SFML "Use find_package to find SFML" OFF)
option(IMGUI_SFML_IMGUI_DEMO "Build imgui_demo.cpp" ON)
FetchContent_MakeAvailable(imgui-sfml)

add_executable(main src/main.cpp
        src/animation.cpp
        headers/animation.hpp
        src/game.cpp
        headers/game.hpp
        src/player.cpp
        headers/player.hpp
        src/inputhandler.cpp
        headers/inputhandler.hpp
        headers/audio.hpp
        src/audio.cpp
        src/perlinnoise.cpp
        src/perlinnoise.cpp
        src/worldgen.cpp
        headers/gameobject.hpp
        headers/gameobject.hpp
        headers/ObjectManager.hpp
        headers/inventory.hpp
        src/inventory.cpp
        headers/item.hpp
        headers/InventoryManager.hpp


)
target_link_libraries(main PRIVATE sfml-graphics sfml-audio sfml-network sfml-window sfml-system sfml-main PUBLIC ImGui-SFML::ImGui-SFML)
target_compile_features(main PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
            TARGET main
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:main>
            VERBATIM)
endif()
