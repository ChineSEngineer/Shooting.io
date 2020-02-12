cmake_minimum_required(VERSION 3.13)
project(asioStudy VERSION 1.0.0 LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 11)
add_compile_options(-Werror)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

find_package(Boost REQUIRED system thread)
find_package(Threads)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../third_party
)


add_executable(game_server game_server.cpp
    websocket_service.cpp
    http_message.cpp
)



target_link_libraries(game_server
    Boost::system
    Boost::thread
)
