cmake_minimum_required(VERSION 3.23)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall -Wextra -pedantic -Og -g")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall -Wextra -pedantic -O3")

project(fractured_lands)
add_executable(${PROJECT_NAME}
  src/colors.cpp
  src/commands.cpp
  src/chat_commands.cpp
  src/combat_commands.cpp
  src/connection_info.cpp
  src/creature.cpp
  src/mongo_controller.cpp
  src/main.cpp
  src/map_commands.cpp
  src/movement_commands.cpp
  src/information_commands.cpp
  src/perlin.cpp
  src/player.cpp
  src/room.cpp
  src/server.cpp
  src/social_commands.cpp
  src/sha256.cpp
  src/utilities.cpp
  src/utility_commands.cpp
  src/world.cpp
)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(fractured_lands PRIVATE fmt::fmt)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(fractured_lands PRIVATE spdlog::spdlog)

find_package(bsoncxx CONFIG REQUIRED)
target_link_libraries(fractured_lands PRIVATE mongo::bsoncxx_static)
find_package(mongocxx CONFIG REQUIRED)
target_link_libraries(fractured_lands PRIVATE mongo::mongocxx_static)
